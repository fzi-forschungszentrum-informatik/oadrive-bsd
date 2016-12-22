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
 * \date    2015-11-01
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_OBSTACLES_PROCESSUSSENSOR_H
#define OADRIVE_OBSTACLES_PROCESSUSSENSOR_H
#include <opencv2/core/core.hpp>
#include <oadrive_obstacle/ProcessUS.h>
#include <vector>
#include <oadrive_core/ExtendedPose2d.h>
#include <oadrive_world/Obstacle.h>
#include <oadrive_util/CoordinateConverter.h>
#include <oadrive_obstacle/ProcessDepth.h>
namespace oadrive{
namespace obstacle{

class ProcessSensors
{
public:
  ProcessSensors(std::string depthRefImage,std::string usCal, util::CoordinateConverter *converter);
  //! \brief ProcessDepthSensor Processes the Depth image and add found objects to the enviroment
  //! \param image depthimage
  void processDepthSensor(cv::Mat image);

  void setNewUsSensorValues(usSensor sensorValues);
  void processUsSensor();
  void setUsSensorBufferLengtg( int length ) { mUsSensorBufferLength = length; }
  usSensor getQuantilesOfRecentUsSensorValues();
  ProcessDepth* getDepthImageProcessor(){return &mProcessDepth;}
  void mergeObjectsToEnviroment(ExtendedPose2dVectorPtr objects, oadrive::world::SensorType sensorType);
  void convertObjectsToWorld(ExtendedPose2dVectorPtr &objects, const ExtendedPose2d &carPos);

  void setFree(oadrive::world::SensorType sensor);

private:

  oadrive::util::CoordinateConverter *mConverter;
  ProcessDepth mProcessDepth;
  ProcessUS mProcessUS;

  //! FIFO buffer for ultrasonic values:
  std::vector<usSensor> mRecentUsSensors;
  size_t mUsSensorBufferLength;

  boost::mutex mtxUs;   // Mutex lock for US Sensors

public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};
}
}

#endif // OADRIVE_OBSTACLES_PROCESSUSSENSOR_H
