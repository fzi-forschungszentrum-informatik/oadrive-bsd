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
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2016-01-17
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_WORLD_EVENT_REGION_H
#define OADRIVE_WORLD_EVENT_REGION_H

#include "EnvObject.h"
#include <oadrive_util/CoordinateConverter.h> 
#include <boost/shared_ptr.hpp>
#include <oadrive_core/ExtendedPose2d.h>

namespace oadrive{
namespace world{

const float CROSS_SECTION_HALT_OFFSET = (CAR_ORIGIN_TO_FRONT + 0.16 + 0.05)*2.0;
const float CROSS_SECTION_BLINK_OFFSET = CROSS_SECTION_HALT_OFFSET + 1.0;
const float CROSS_SECTION_CENTER_OFFSET = 0.4*2.0;
const float UNCONNECTED_TRAFFIC_SIGN_REGION_SIZE = 2.8;
const float UNCONNECTED_TRAFFIC_SIGN_OFFSET = UNCONNECTED_TRAFFIC_SIGN_REGION_SIZE*0.5 - 0.1;

enum EventRegionType { CROSS_SECTION_HALT, CROSS_SECTION_BLINK, CROSS_SECTION_CENTER, PARKING_SIGN,
                       PARKING_PARALLEL, OBSTACLE_REGION, OBSTACLE_REGION_SMALL, OBSTACLE_PASSED_REGION, OVERTAKE_FINISHED_REGION , PARKING_CROSS, CROSS_SECTION_OBSTACLES,
                       UNCONNECTED_TRAFFIC_SIGN };
enum EventType { EVENT_ENTERED_REGION, EVENT_EXITED_REGION };

class EventRegion;	// predefine so shared_ptr can use it
typedef boost::shared_ptr<EventRegion> EventRegionPtr;		// define before class because it uses the PatchPtr
typedef std::list<boost::shared_ptr<EventRegion> > EventRegionPtrList;

class EventRegion : public EnvObject
{
public:
  EventRegion( EventRegionType mType, const oadrive::core::ExtendedPose2d &mPose,
               float mWidth, float mHeight );
  ~EventRegion();

  EnvObjectPtr getObjectOfInterest() { return mObjectOfInterest; }
  void setObjectOfInterest( EnvObjectPtr object ) { mObjectOfInterest = object; }

  bool getIsCarInside() { return mIsCarInside; }
  void setIsCarInside( bool isInside ) { mIsCarInside = isInside; }

  EventRegionType getEventRegionType() { return mEventRegionType; }

  oadrive::core::ExtendedPose2d getLocalPose() { return mLocalPose; }

    bool isToDelete() const {
    return mToDelete;
    }

    void setToDelete(bool toDelete) {
    EventRegion::mToDelete = toDelete;
    }

private:

  EnvObjectPtr mObjectOfInterest;

  EventRegionType mEventRegionType;

  bool mIsCarInside;


private:
    bool mToDelete;

  oadrive::core::ExtendedPose2d mLocalPose;
public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}	// namespace
}	// namespace

#endif
