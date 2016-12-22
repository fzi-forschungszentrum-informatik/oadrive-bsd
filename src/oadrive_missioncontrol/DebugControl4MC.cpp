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
#include "DebugControl4MC.h"

#include "oadrive_missioncontrol/mcLogging.h"
using icl_core::logging::endl;
using icl_core::logging::flush;

namespace oadrive{
namespace missioncontrol{

void DebugControl4MC::setJuryState (stateCar state, int manID) {
        lastState = state;
        lastManID = manID;
    //TODO State als Beschreibung
    LOGGING_INFO( mcLogger, "DummyControl4MC State: " << state << "with manID "<< manID << endl );

}

oadrive::util::Timer* DebugControl4MC::getTimer() {
        return &timer;
}

void DebugControl4MC::setLights(enumLight light, bool on) {
        lastLight = light;
        lastOn = on;
        if(on) {
                LOGGING_INFO( mcLogger, "DummyControl4MC turning on light: " << light << endl);
        } else {
                LOGGING_INFO( mcLogger, "DummyControl4MC turning off light: " << light << endl);
        }
}

void DummyDriverControl::drive() {
        LOGGING_INFO( mcLogger, "DummyDriverControl drive() method." << endl);

}

void DummyDriverControl::halt() {
        LOGGING_INFO( mcLogger, "DummyDriverControl halt() method." << endl);
}

void DummyDriverControl::setTargetSpeed(float speed) {
        lastSpeed = speed;
        LOGGING_INFO( mcLogger, "DummyDriverControl setTargetSpeed(" << speed <<")." << endl);
}

void DummyStreetPatcher::reset() {
        LOGGING_INFO( mcLogger, "DummyStreetPatcher reset() method." << endl);
}

bool DummyTrajectoryFactory::generateFromPatches() {
        LOGGING_INFO( mcLogger, "DummyTrajectoryFactory generateFromPatches() method." << endl);
        return true;
}

void DummyTrajectoryFactory::setFixedTrajectory(enumTrajectory man,
                oadrive::world::PatchPtr patch ) {
        lastMan = man;
        LOGGING_INFO( mcLogger, "DummyTrajectoryFactory setFixedTrajectory(" << man <<")." << endl);
}

}	// namespace
}	// namespace

*/
