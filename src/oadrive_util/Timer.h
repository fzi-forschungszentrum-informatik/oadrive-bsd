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
 * \author Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2016-01-22
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_UTIL_TIMER_H
#define OADRIVE_UTIL_TIMER_H
#include <list>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "TimerTypes.h"
#include "TimerEventListener.h"

namespace oadrive {
namespace util {

struct structTimers{
  int endTime;
  unsigned long uniqueTimerID;
  timerType type;
  bool markedForDeletion;
};
class Timer
{
public:
  Timer();
  //!
  //! \brief setTimeIncrement Increments the internal counter
  //! \param milliSeconds milliSeconds which elapsed since last call
  //!
  void setTimeIncrement(int milliSeconds);

  //!\brief returns the time in milli seconds since system start
  int getTime();
  //!
  //! \brief setTimer add a timer. funcPtr will be call after timeMilliseconds. It is not garanteed, that funcPtr is called exact after N milliseconds.
  //! \note delay depens on setTimeIncrement. It is only guranted, that it is called afte time Millisenconds.
  //! \param funcPtr Ptr to function (can be produced with boost::bind( &testclass::testFunc2, this ))
  //! \param timeMilliseconds time in Milliseconds after that funcPtr will be called
  //! \return uniqueTimerID, can be used to identify the timer later.
  //!
  unsigned long setTimer(int timeMilliseconds, timerType type );

  /*! Sets the timer given by timerID to be deleted.*/
  void removeTimer( unsigned long timerID );

  void addListener( TimerEventListener* listener );
  void removeListener( TimerEventListener* listener );

  void clearAllTimers();

private:
  //! \brief mTime holds the time in milliSeconds. Overflow in 8 Years. Happy debugging.
  int mTime;
  //!brief holds the maximum ID
  unsigned long mMaxId;
  //!\brief holds all actual timers
  std::list <structTimers> mTimers;
  //!\brief checks all timers if a call is needed
  void checkTimers();

  std::list<TimerEventListener*> mListeners;
};
}
}
#endif // TIMER_H
