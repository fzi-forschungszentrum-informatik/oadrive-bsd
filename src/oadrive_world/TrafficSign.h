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
 * \author  David Zimmerer <dzimmerer@gmail.com>
 * \author  Jan-Markus Gomer <uecxl@student.kit.edu>
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2016-01-19
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_WORLD_TRAFFICSIGN_H
#define OADRIVE_WORLD_TRAFFICSIGN_H

#include "EnvObject.h"
#include <oadrive_util/CoordinateConverter.h> 
#include <boost/shared_ptr.hpp>
#include <oadrive_core/ExtendedPose2d.h>
#include "Patch.h"
#include "Obstacle.h"
#include <list>


namespace oadrive{
namespace world{

//! probability added to each detection of the same traffic sign
const float TRAFFIC_SIGN_PROBABILITY_ADD = 0.1;
//! probability must be greater than set value for a traffic sign to connect to a special patch (0.1 means, at least 2 detections)
const float TRAFFIC_SIGN_MINIMUM_CONNECTION_PROBABLITY = 0.1;
//! merging distance for traffic signs in meters
const float TRAFFIC_SIGN_MERGING_DIST = 0.6;
//! Maximum allowed distance between traffic sign and center point of CROSS_SECTION:
const float TRAFFIC_SIGN_MAX_DIST_TO_CROSS_SECTION = 1.5;
//! Usual distance from center point of CROSS_SECTION and traffic sign:
const float TRAFFIC_SIGN_DIST_TO_CROSS_SECTION = 1.031;
//! Maximum allowed distance between traffic sign and center point of STRAIGHT patch:
const float TRAFFIC_SIGN_MAX_DIST_TO_STRAIGHT = 0.9;

//! Signs detected beyond this distance to the car will be ignored (they're often inexact).
const float TRAFFIC_SIGN_DETECTION_RADIUS = 4.5;

class TrafficSign;	// predefine so shared_ptr can use it
typedef boost::shared_ptr<TrafficSign> TrafficSignPtr;		// define before class because it uses the PatchPtr
typedef std::list<boost::shared_ptr<TrafficSign> > TrafficSignPtrList;

class Patch;	// predefine so shared_ptr can use it
typedef boost::shared_ptr<Patch> PatchPtr;		// define before class because it uses the PatchPtr
typedef std::list<boost::shared_ptr<Patch> > PatchPtrList;

class TrafficSign : public EnvObject
{
public:
  TrafficSign( int type, double x, double y );
  TrafficSign( int type, const oadrive::core::ExtendedPose2d &pose );
  ~TrafficSign();

  PatchPtr getConnectedPatch() { return mConnectedPatch; }
  void setConnectedPatch( PatchPtr patch ) { mConnectedPatch = patch; }

  int getSignType() { return mTrafficSignType; }


private:

  PatchPtr mConnectedPatch;
  int mTrafficSignType;
public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

}	// namespace
}	// namespace

#endif
