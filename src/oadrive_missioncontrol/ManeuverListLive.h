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

#ifndef PACKAGES_OADRIVE_SRC_OADRIVE_MISSIONCONTROL_MANEUVERLISTLIVE_H_
#define PACKAGES_OADRIVE_SRC_OADRIVE_MISSIONCONTROL_MANEUVERLISTLIVE_H_

#include "IManeuverList.h"
#include "IMC2Man.h"
#include <vector>
#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/assert.hpp>
#include <map>

namespace oadrive
{
  namespace missioncontrol
  {

    using namespace std;

    class ManeuverListLive : public IManeuverList
    {
    public:
      ManeuverListLive ();
      void increase();
      std::string toString();
      unsigned int getCurrentAbsManeuverID();
      enumManeuver getCurrentManeuver();
      enumManeuver getPreviosManeuver();
      bool isFinished();
      bool setManeuverId(int maneuverEntryID);
      bool isDummy();
      IMC2Man* mc;
      void onMessage(const std::vector<char> &buf);
      bool isLive() { return true; }

    private:
      enumManeuver m_currentMan;
      enumManeuver m_prevMan;
      unsigned int m_manID;
      bool m_isFin;
      static map<string, enumManeuver> maneuverMap;
    };

  } /* namespace missioncontrol */
} /* namespace oadrive */

#endif /* PACKAGES_OADRIVE_SRC_OADRIVE_MISSIONCONTROL_MANEUVERLISTLIVE_H_ */
