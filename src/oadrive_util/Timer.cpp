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
 * \author  Peter Zimmer <peter-zimmer@gmx.net>
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2016-01-22
 *
 */
//----------------------------------------------------------------------

#include "Timer.h"

#include "utilLogging.h"
using icl_core::logging::endl;
using icl_core::logging::flush;

namespace oadrive {
namespace util {
Timer::Timer()
  : mTime(0)
  , mMaxId(1)
{
}

void Timer::setTimeIncrement(int milliSeconds)
{
  mTime += milliSeconds;
  checkTimers();
}

int Timer::getTime()
{
  return mTime;
}

unsigned long Timer::setTimer(int timeMilliseconds, timerType type )
{
  structTimers newTimer;
  newTimer.endTime = mTime+timeMilliseconds;
  newTimer.type = type;
  mMaxId++;
  newTimer.uniqueTimerID = mMaxId;
  newTimer.markedForDeletion = false;
  mTimers.push_back(newTimer);

//  LOGGING_INFO( utilLogger, "New timer registered (type " << type << ")" << endl );

  return newTimer.uniqueTimerID;
}

void Timer::removeTimer( unsigned long timerID )
{
  std::list<structTimers>::iterator timerIt = mTimers.begin();
  for( timerIt = mTimers.begin(); timerIt != mTimers.end(); timerIt++ )
  {
    if( timerIt->uniqueTimerID == timerID )
    {
      // There's a timer with this ID, so mark it for deletion:
      timerIt->markedForDeletion = true;
      return;
    }
  }
}

void Timer::checkTimers()
{
  std::list<structTimers>::iterator timerIt = mTimers.begin();
  while(timerIt != mTimers.end())
  {
    // First, check if the timer is to be deleted:
    if( timerIt->markedForDeletion )
    {
      LOGGING_INFO( utilLogger, "Deleting timer of type " << timerIt->type << endl );
      // Remove the timer:
      timerIt = mTimers.erase( timerIt );
    } else if( timerIt->endTime <= mTime ) {
      // If the timer should not be deleted and its time has passed, fire the event.
//      LOGGING_INFO( utilLogger, "Firing timer of type " << timerIt->type << endl );

      std::list<TimerEventListener*>::iterator listenerIt;
      for( listenerIt = mListeners.begin(); listenerIt != mListeners.end(); listenerIt++ )
      {
        // Notify the listeners:
        (*listenerIt)->eventTimerFired( timerIt->type, timerIt->uniqueTimerID );
      }
      // Remove the timer:
      timerIt = mTimers.erase(timerIt);
    } else {
      timerIt++;
    }
  }
}

void Timer::addListener( TimerEventListener* listener )
{
//  LOGGING_INFO( utilLogger, "New timer event listener registered." << endl );
  mListeners.push_back( listener );
}

void Timer::removeListener( TimerEventListener* listener )
{
  mListeners.remove( listener );
}

void Timer::clearAllTimers()
{
  mTimers.clear();
}

}
}
