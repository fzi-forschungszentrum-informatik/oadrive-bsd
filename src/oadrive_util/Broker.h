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

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/address.hpp>
#include <redisclient/redisasyncclient.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <oadrive_missioncontrol/MissionControlEnums.h>

#ifndef PACKAGES_OADRIVE_SRC_OADRIVE_UTIL_BROKER_H_
#define PACKAGES_OADRIVE_SRC_OADRIVE_UTIL_BROKER_H_

namespace oadrive
{
namespace util
{

enum ChannelEnum {
  CHANNEL_SEND_STATUS,
  CHANNEL_RECEIVE_COMMAND,
  CHANNEL_SEND_DISTANCE,
  CHANNEL_SEND_MAP,
  CHANNEL_SEND_CHECKPOINT,
  CHANNEL_SEND_CROSSING
};

enum enumSpeedCommand {
  SPEED_COMMAND_NONE,
  SPEED_COMMAND_BOOST,
  SPEED_COMMAND_NORMAL
};

class Broker;   // predefine for typedef.
typedef boost::shared_ptr<Broker> BrokerPtr;

class Broker
{
  public:
    static BrokerPtr getInstance();
    static bool isActive();
    void publish( ChannelEnum channel, std::string msg );
    enumManeuver getLastReceivedManeuver();
    bool getTurnCommandReceived();
    enumSpeedCommand getLastSpeedCommand();
    bool getHasReceivedStopCommand();
    ~Broker();

  private:
    Broker();
    static boost::shared_ptr<Broker> mInstance;

    bool mConnected; //Do not use directly because of threading! Use methods instead
    const unsigned short mPort;

    boost::asio::io_service mIOService;
    RedisAsyncClient mSubscriber;
    RedisAsyncClient mPublisher;

    std::string mCommandsReceiveChannel;
    std::string mStatusSendChannel;

    bool isConnected();
    void setConnected(bool b);


    void handleConnectedEventSub(boost::asio::io_service &ioService, RedisAsyncClient &redis,
        bool ok, const std::string &errmsg);
    void handleConnectedEventPub(boost::asio::io_service &ioService, RedisAsyncClient &redis,
        bool ok, const std::string &errmsg);
    void eventReceivedCommand(boost::asio::io_service &ioService, const std::vector<char> &buf);

    void setLastReceivedManeuver( std::string msg );
    void setTurnCommandReceived();
    void setNewSpeedCommand( std::string msg );
    void setHasReceivedStopCommand();

    enumManeuver mLastReceivedManeuver;
    bool mReceivedTurnCommand;
    enumSpeedCommand mLastSpeedCommand;
    bool mGameFinished;

    boost::mutex mtx;

    std::map<ChannelEnum, std::string> mChannelNames;

};

} /* namespace missioncontrol */
} /* namespace oadrive */

#endif /* PACKAGES_OADRIVE_SRC_OADRIVE_UTIL_BROKER_H_ */
