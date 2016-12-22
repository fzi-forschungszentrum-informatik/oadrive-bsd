// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// Copyright (c) 2016, FZI Forschungszentrum Informatik
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \author  Vitali Kaiser <vitali.kaiser@live.de>
 * \date    2016-03-03
 *
 */
//----------------------------------------------------------------------

#include "Broker.h"
#include "utilLogging.h"
#include <oadrive_util/Config.h>
#include <boost/thread/thread.hpp>
#include <algorithm>
using icl_core::logging::endl;
using icl_core::logging::flush;

namespace oadrive
{
namespace util
{

boost::shared_ptr<Broker> Broker::mInstance;

// Called when new direction for next intersection is received:
void Broker::eventReceivedCommand(
    boost::asio::io_service &ioService, const std::vector<char> &buf )
{
  std::string msg(buf.begin(), buf.end());

  LOGGING_INFO( utilLogger, "Received new command: " << msg << endl );

  if( msg == "stop" )   // End Redis connection
    ioService.stop();
  else if( msg == "left" || msg == "right" || msg == "straight" )
    setLastReceivedManeuver( msg );
  else if( msg == "turn" )
    setTurnCommandReceived();
  else if( msg == "speed_boost" || msg == "speed_normal" )
    setNewSpeedCommand( msg );
  else if( msg == "game_finished" )
    setHasReceivedStopCommand();
}

void Broker::handleConnectedEventSub(
    boost::asio::io_service &ioService, RedisAsyncClient &redis,
    bool ok, const std::string &errmsg)
{
  if( ok )
  {
    mSubscriber.subscribe( mChannelNames[CHANNEL_RECEIVE_COMMAND],
        boost::bind( &Broker::eventReceivedCommand, this, boost::ref(ioService), _1) );
    LOGGING_INFO( utilLogger, "Subscribed to channel: " << endl );
    LOGGING_INFO( utilLogger, "\t" << mChannelNames[CHANNEL_RECEIVE_COMMAND] << endl );
    setConnected(true);
  }
  else
  {
    LOGGING_ERROR( utilLogger, "Broker could not connect to redis!" << endl );
  }
}

void Broker::handleConnectedEventPub(
    boost::asio::io_service &ioService, RedisAsyncClient &redis,
    bool ok, const std::string &errmsg)
{
  if( ok )
  {
    LOGGING_INFO( utilLogger, "Publisher connected." << endl );            

    publish( CHANNEL_SEND_STATUS, "connected" );
  }
  else
  {
    LOGGING_ERROR( utilLogger, "Broker could not connect to redis!" << endl );
  }
}



Broker::Broker ()
  : mConnected( false )
  , mPort( 6379 )
  , mIOService()
  , mSubscriber( mIOService )
  , mPublisher( mIOService )
  , mLastReceivedManeuver( MANEUVER_RIGHT )
  , mReceivedTurnCommand( false )
  , mLastSpeedCommand( SPEED_COMMAND_NONE )
  , mGameFinished( false )
{

  LOGGING_INFO( utilLogger, "Broker constructed." << endl );

  std::string ip = Config::getString( "Kuer", "ServerIP", "10.0.0.1" );
  boost::asio::ip::address address = boost::asio::ip::address::from_string( ip );

  LOGGING_INFO( utilLogger, "Connecting broker to: " << ip << ":" << mPort << endl );            
  std::string carName = Config::getCarName();
  std::transform(carName.begin(), carName.end(), carName.begin(), ::tolower);

  mChannelNames.insert( std::pair<ChannelEnum, std::string>(
        CHANNEL_RECEIVE_COMMAND, "oadrive/" + carName + "/commands") );
  mChannelNames.insert( std::pair<ChannelEnum, std::string>(
        CHANNEL_SEND_STATUS, "oadrive/" + carName + "/info/status") );
  mChannelNames.insert( std::pair<ChannelEnum, std::string>(
        CHANNEL_SEND_DISTANCE, "oadrive/" + carName + "/info/distance") );
  mChannelNames.insert( std::pair<ChannelEnum, std::string>(
        CHANNEL_SEND_MAP, "oadrive/" + carName + "/info/map") );
  mChannelNames.insert( std::pair<ChannelEnum, std::string>(
      CHANNEL_SEND_CHECKPOINT, "oadrive/" + carName + "/info/checkpoint") );
  mChannelNames.insert( std::pair<ChannelEnum, std::string>(
      CHANNEL_SEND_CROSSING, "oadrive/" + carName + "/info/crossing") );

  std::map<ChannelEnum,std::string>::iterator it;
  for( it = mChannelNames.begin(); it != mChannelNames.end(); it++ )
  {
    LOGGING_INFO( utilLogger, "Channel: " << it->first << ") " << it->second << endl );
  } 

  mSubscriber.asyncConnect( address, mPort,
      boost::bind(
        &Broker::handleConnectedEventSub, this,
        boost::ref( mIOService ),
        boost::ref( mSubscriber ),
        _1, _2 ) );

  mPublisher.asyncConnect( address, mPort,
      boost::bind(
        &Broker::handleConnectedEventPub, this,
        boost::ref( mIOService ),
        boost::ref( mSubscriber ),
        _1, _2 ) );

  //mIOService.run();
  // Run in seperate thread:
  boost::thread ioThread(boost::bind(&boost::asio::io_service::run, &mIOService));
}

Broker::~Broker()
{
  publish( CHANNEL_SEND_STATUS, "disconnected" );
}

BrokerPtr Broker::getInstance()
{
  if(!mInstance) {
    mInstance = boost::shared_ptr<Broker>(new Broker());
  }
  return mInstance;
}

void Broker::publish( ChannelEnum channel, std::string msg )
{
  if( !isConnected() ) return;

  if( msg.size() < 20 )
  {
    LOGGING_INFO( utilLogger, "Publishing: " << endl <<
        "\t" << mChannelNames[channel] << ": \"" << msg << "\"" << endl );
  } else {
    LOGGING_INFO( utilLogger, "Publishing: " << endl <<
        "\t" << mChannelNames[channel] << " (long message)" << endl );
  }
  mPublisher.publish( mChannelNames[channel], msg );
}

bool Broker::isConnected()
{
  bool connected;
  mtx.lock();
  connected = mConnected;
  mtx.unlock();
  return connected;
}

void Broker::setConnected(bool b)
{
  mtx.lock();
  mConnected = b;
  mtx.unlock();
}


void Broker::setLastReceivedManeuver( std::string msg )
{
  mtx.lock();
  if( msg == "left" )
  {
    mLastReceivedManeuver = MANEUVER_LEFT;
  } else if( msg == "straight" ) {
    mLastReceivedManeuver = MANEUVER_STRAIGHT;
  } else {
    mLastReceivedManeuver = MANEUVER_RIGHT;
  }
  mtx.unlock();
}

enumManeuver Broker::getLastReceivedManeuver()
{
  enumManeuver tmp;
  mtx.lock();
  tmp = mLastReceivedManeuver;
  mtx.unlock();
  return tmp;
}

void Broker::setTurnCommandReceived()
{
  mtx.lock();
  mReceivedTurnCommand = true;
  mtx.unlock();
}
bool Broker::getTurnCommandReceived()
{
  bool tmp;
  mtx.lock();
  tmp = mReceivedTurnCommand;
  mReceivedTurnCommand = false;
  mtx.unlock();
  return tmp;
}
void Broker::setNewSpeedCommand( std::string msg )
{
  mtx.lock();
  if( msg == "speed_boost" )
  {
    mLastSpeedCommand = SPEED_COMMAND_BOOST;
  } else if( msg == "speed_normal" ) {
    mLastSpeedCommand = SPEED_COMMAND_NORMAL;
  } else {
    mLastSpeedCommand = SPEED_COMMAND_NONE;
  }
  mtx.unlock();
}
enumSpeedCommand Broker::getLastSpeedCommand()
{
  enumSpeedCommand command;
  mtx.lock();
  command = mLastSpeedCommand;
  mLastSpeedCommand = SPEED_COMMAND_NONE;   // reset
  mtx.unlock();
  return command;
}
void Broker::setHasReceivedStopCommand()
{
  mtx.lock();
  mGameFinished = true;
  mtx.unlock();
}
bool Broker::getHasReceivedStopCommand()
{
  bool tmp;
  mtx.lock();
  tmp = mGameFinished;
  mGameFinished = false;
  mtx.unlock();
  return tmp;
}


bool Broker::isActive()
{
  if( mInstance )
  {
    if( mInstance->isConnected() )
    {
      return true;
    }
  }
  return false;
}

}   // namespace
}   // namespace
