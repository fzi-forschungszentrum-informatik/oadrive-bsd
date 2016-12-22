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
 * \date    2016-01-16
 *
 */
//----------------------------------------------------------------------

#ifndef PACKAGES_OADRIVE_SRC_OADRIVE_MISSIONCONTROL_MISSIONCONTROLENUMS_H_
#define PACKAGES_OADRIVE_SRC_OADRIVE_MISSIONCONTROL_MISSIONCONTROLENUMS_H_



/**
* Left				An der nächsten Kreuzung links abbiegen
* Right				An der nächsten Kreuzung rechts abbiegen
* Straight			An der nächsten Kreuzung geradeaus fahren
* parallel_parking	Am nächsten Parkplatz in Fahrtrichtung rechts längs einparken
* cross_parking		Am nächsten Parkplatz in Fahrtrichtung rechts quer einparken
* pull_out_left		Parkplatz nach links verlassen (vom Parkplatz senkrecht in Richtung Straße blickend, nur in Verbindung mit Querparken)
* pull_out_right	Parkplatz nach rechts verlassen (vom Parkplatz senkrecht in Richtung Straße blickend)
* finished			!! Selbst definiert. Der nächste Job ist quasi stehen bleiben.
 */
enum enumManeuver {MANEUVER_LEFT, MANEUVER_RIGHT, MANEUVER_STRAIGHT, MANEUVER_PARKING_PARALLEL,
	MANEUVER_PARKING_CROSS, MANEUVER_PULLOUT_LEFT, MANEUVER_PULLOUT_RIGHT, MANEUVER_FINISHED};

enum enumLight { HEAD_LIGHT, REVERSE_LIGHT, BRAKE_LIGHT, BLINK_RIGHT_LIGHT, BLINK_LEFT_LIGHT,
	HAZARD_LIGHT };

#endif /* PACKAGES_OADRIVE_SRC_OADRIVE_MISSIONCONTROL_MISSIONCONTROLENUMS_H_ */
