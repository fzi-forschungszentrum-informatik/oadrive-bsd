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
 * \author  Vitali Kaiser <vitali.kaiser@live.de>
 * \date    2016-02-27
 *
 */
//----------------------------------------------------------------------

#include "ManeuverListLive.h"
#include <string>
#include <iostream>
#include <oadrive_util/Broker.h>

//next 3 lines are for logging
#include "oadrive_missioncontrol/mcLogging.h"
using icl_core::logging::endl;
using icl_core::logging::flush;

using namespace oadrive::util;

namespace oadrive
{
  namespace missioncontrol
  {
    using namespace boost::assign;
    static const std::string channelName = "oadrive/manlist";


    map<string, enumManeuver> ManeuverListLive::maneuverMap = map_list_of ("left", MANEUVER_LEFT) ("right", MANEUVER_RIGHT) ("straight", MANEUVER_STRAIGHT) ("parallel_parking", MANEUVER_PARKING_PARALLEL) ("cross_parking", MANEUVER_PARKING_CROSS) ("pull_out_left", MANEUVER_PULLOUT_LEFT) ("pull_out_right", MANEUVER_PULLOUT_RIGHT);


    ManeuverListLive::ManeuverListLive ()
    {
      m_currentMan = MANEUVER_RIGHT;
      m_prevMan = MANEUVER_RIGHT;
      m_manID = 0;
      m_isFin = false;
      mc = 0;

      LOGGING_INFO( mcLogger, "ManeuverListLive constructed." << endl );

      // Initialize the broker:
      Broker::getInstance();
    }

    void ManeuverListLive::onMessage(const std::vector<char> &buf) {
      std::string msg(buf.begin(), buf.end());
    }


    void ManeuverListLive::increase() {
      m_manID++;
    }

    std::string ManeuverListLive::toString() {
      return "";
    }
    unsigned int ManeuverListLive::getCurrentAbsManeuverID() {
      return m_manID;
    }

    enumManeuver ManeuverListLive::getCurrentManeuver() {
      // When asked for a new manevuer, simply retrieve the last received maneuver:
      m_currentMan = Broker::getInstance()->getLastReceivedManeuver();
      LOGGING_INFO( mcLogger, "Current maneuver in ManeuverListLive: " << m_currentMan << endl );
      return m_currentMan;
    }

    enumManeuver ManeuverListLive::getPreviosManeuver() {
      return m_prevMan;
    }
    bool ManeuverListLive::isFinished() {
      return m_isFin;
    }
    bool ManeuverListLive::setManeuverId(int maneuverEntryID) {
      //ignore that... we dont have ids.....
      return true;
    }
    bool ManeuverListLive::isDummy() {
      // we are the real thing ;)
      return false;
    }

  } /* namespace missioncontrol */
} /* namespace oadrive */
