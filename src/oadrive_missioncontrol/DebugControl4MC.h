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
 * \date    2015-12-16
 *
 */
//----------------------------------------------------------------------
/*
#ifndef MISSIONCONTROL_DEBUGCONTROL4MC_H
#define MISSIONCONTROL_DEBUGCONTROL4MC_H

#include "IControl4MC.h"
#include "../oadrive_control/IDriverModule.h"
#include "../oadrive_lanedetection/IStreetPatcher.h"
#include "../oadrive_world/IEnvironment.h"
#include "../oadrive_world/ITrajectoryFactory.h"
#include <iostream>

namespace oadrive{
namespace missioncontrol{

//Outputs everything to console
class DebugControl4MC : public IControl4MC {
	public:
	void setJuryState(stateCar state, int manID);
	stateCar lastState;
	int lastManID;

	void setLights(enumLight light, bool on);
	enumLight lastLight;
	bool lastOn;

	oadrive::util::Timer* getTimer();

	oadrive::util::Timer timer;

};

class DummyDriverControl: public IDriverModule {
public:
	void drive();
	void halt();
	void setTargetSpeed(float speed);
	float lastSpeed;
};

class DummyStreetPatcher: public IStreetPatcher {
public:
	void reset();
	oadrive::world::ParkingType getParkingSpaceDirection( cv::Mat &image ) {
		return PARKING_TYPE_UNKNOWN;
	}
};

class DummyEnvironment: public IEnvironment {

};

class DummyTrajectoryFactory: public ITrajectoryFactory {
	enumTrajectory lastMan;
	void setFixedTrajectory( enumTrajectory trajName,
			oadrive::world::PatchPtr patch = oadrive::world::PatchPtr() );
	void setGenerateFromPatches() {};
	bool generateFromPatches();
};


}	// namespace
}	// namespace

#endif //MISSIONCONTROL_DEBUGCONTROL4MC_H
/*
