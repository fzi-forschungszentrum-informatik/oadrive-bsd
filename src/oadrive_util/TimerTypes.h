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
 * \author Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2016-02-01
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_UTIL_TIMERTYPES_H
#define OADRIVE_UTIL_TIMERTYPES_H

namespace oadrive {
namespace util {

enum timerType { TIMER_TYPE_PARKING, TIMER_TYPE_HALTING_AT_CROSSING, TIMER_TYPE_REMOVE_OBSTACLES,
                 TIMER_TYPE_LATERALCONTROLLER , TIMER_TYPE_REMOVE_OLD_OBJECTS,
                 TIMER_TYPE_HALTING_AT_OBSTACLE, TIMER_TYPE_CHECKING_AT_OBSTACLE,
                 TIMER_TYPE_CHECK_TRAJECTORY_FREE, TIMER_TYPE_CHECK_TRAJECTORY_FREE_AGAIN,
                 TIMER_TYPE_RETURN_AFTER_OBSTACLE, TIMER_TYPE_DRIVE_WITHOUT_TRAJECTORY,
                 TIMER_TYPE_SEND_MAP, TIMER_SET_SMALL_FORWARD_TRAJ };

} // namespace
} // namespace

#endif // OADRIVE_UTIL_TIMERTYPES_H
