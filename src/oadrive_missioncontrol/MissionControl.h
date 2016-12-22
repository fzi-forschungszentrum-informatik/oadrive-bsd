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
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2015-12-03
 *
 */
//----------------------------------------------------------------------

#ifndef MISSIONCONTROL_MISSIONCONTROL_H
#define MISSIONCONTROL_MISSIONCONTROL_H

/*
//if the state machine has more than 10 states
#define FUSION_MAX_VECTOR_SIZE 20 // or whatever you need


//If you neeed more than 20 transition in the state machine
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 30 //or whatever you need
#define BOOST_MPL_LIMIT_MAP_SIZE 30 //or whatever you need
 */

//#include "Environnement.h"


#include <iostream>
#include "IControl4MC.h"
#include "juryEnums.h"
#include <oadrive_world/WorldEventListener.h>
#include <oadrive_world/Patch.h>
#include <oadrive_world/EventRegion.h>
#include <oadrive_world/Environment.h>
#include <oadrive_world/TrajectoryFactory.h>
#include <oadrive_lanedetection/StreetPatcher.h>
#include <oadrive_control/DriverModule.h>
#include <oadrive_util/TimerEventListener.h>
#include "IMC2Man.h"

//state-machine
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/tools.hpp>
#include <boost/mpl/vector/vector50.hpp>

#include <iostream>
#include "ManeuverList.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <oadrive_missioncontrol/mcLogging.h>
using icl_core::logging::endl;
using icl_core::logging::flush;

//#include <boost/asio.hpp>
//#include <boost/bind.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

#include <map>

#include "StateMachine.h"

namespace oadrive{
namespace missioncontrol{
using namespace oadrive::world;
using namespace oadrive::control;
using namespace oadrive::lanedetection;
using namespace oadrive::util;

class MissionControl : public WorldEventListener, public TimerEventListener, public IMC2Man {

  struct juryActionsTupel {
    juryActions action;
    int manId;
  };

public:
  MissionControl(IControl4MC* controller,
                 DriverModule* driverModule = NULL,
                 TrajectoryFactory* trajectoryFactory = NULL,
                 StreetPatcher* streetPatcher = NULL );

  void eventJurySignalReceived (juryActions ja, int maneuverEntryID );

  /*! Receive the (unparsed!) xml maneuver list. */
  void eventManeuverListReceived( std::string maneuverList );
  //! A special Patch (crossing, parking,etc) was found, this function informs the mission control.
  //void eventSpecialPatchFound(PatchPtr specialPatch);

  //! Called when the StreetPatcher has been initialized.
  void eventStreetPatcherIsInitialized() {};

  //! Called when a region is entered or left.
  void eventRegionTriggered( EventRegionPtr region, EventType evType );

  //! Returns the representing id of the current state
  const int* getCurrentStateId();

  //! event when trajectory is reached
  void eventTrajectoryEndReached();

  //! event when empty trajectory is set
  void eventTrajectoryEmpty();

  //! event when there are enough patches to create a traj.
  void eventReadyToGeneratePatchTrajectory();
  //void eventCannotGeneratePatchTrajectory();

  //! is called when we were halting and it can go on
  void eventWaitOver();


  //! a section was completed!
  void eventSectionCompleted();

  //! finished event is sent when there is noting left to do in the maneuver list
  void eventParcourFinished();

  //! we have to start searching for parking lot
  void eventSearchParking();

  // Implement TimerEventListener:
  void eventTimerFired( timerType type, unsigned long timerID );

  //! allows to set the jury state from the mission control
  void setJuryState( stateCar state, int manID );

  /*! Starts the U-Turn maneuver (Kuer). MUST be in StateDriving!
   * Otherwise this has no effect. */
  void eventUTurn();

  /*! Lets the MissionControl know that there has been a new boost command (Kuer). */
  void setBoostCommand( enumSpeedCommand command );

private:
  IControl4MC* controller;
  StateMachine mStateMachine;
  TrajectoryFactory* tf;
  DriverModule* dm;
  StreetPatcher* sp;

  std::map<EventRegionPtr,int> currentRegions;

  boost::shared_ptr<IManeuverList> m_maneuverList;




  std::list<PatchPtr> m_crossings;

  bool setPatchtoCurrentManeuver(PatchPtr specialPatch);

};

}	// namespace
}	// namespace

#endif //MISSIONCONTROL_MISSIONCONTROL_H
