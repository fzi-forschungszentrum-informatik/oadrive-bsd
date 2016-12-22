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

#ifndef OADRIVE_OBSTACLE_PROCESSUS_H
#define OADRIVE_OBSTACLE_PROCESSUS_H
#include <opencv2/core/core.hpp>
#include "string.h"
#include <iostream>
#include "time.h"
#include <oadrive_core/ExtendedPose2d.h>
#include <boost/shared_ptr.hpp>
#include <vector>

#define NUMBERCALPOINTSPERSENSOR 2
#define NUMBERSENSOR 10

namespace oadrive{
namespace obstacle{

struct usSensor {
  float frontLeft;
  float frontCenterLeft;
  float frontCenter;
  float frontCenterRight;
  float frontRight;
  float sideLeft;
  float sideRight;
  float rearLeft;
  float rearCenter;
  float rearRight;
};

enum enumUSSensorLimits {
  LIMIT_FOR_CROSSING,
  LIMIT_FOR_SEARCH_PARKING,
  LIMIT_FOR_OVERTAKING,
  LIMIT_FOR_DRIVING,
  LIMIT_DEACTIVATE
};

class ProcessUS
{
public:
  ProcessUS();
  //!\param calFile path to cal file were the postion of the US sensors is stored
  ProcessUS(std::string calFile);
  /*!
    * \brief loadCalPoints Loads the calibration point. Use UltrasonicCal to write the points
    * \param path Path for loading the calibraten points
    */
  void loadCalPoints(std::string path);
  /*!
    \brief transform the US Sensor to the car grid
    \param sensorNumber Number of the sensor start at 0 at the left of the frontbumber. Count clockwise
    \return position of the objekt in Car Grid
    */
  oadrive::core::ExtendedPose2d transformToCar(int sensorNumber, double distance);

  //! \brief getObjects transforms every US sensor in one object
  //! \param sensor struct with all distances in m
  //! \return boost shared Pointer to vector with Extended Poses
  oadrive::core::ExtendedPose2dVectorPtr getObjects(usSensor sensor);

  /*!
   * \brief printCalPoints Print Cal Points to console
   */
  void printCalPoints();
  //!print sensor positions to console
  void printSensorPos();
  //!Save all calibration points
  void saveCal(std::string path);
  //!Set one Calibration Point
  /*!
   * \brief setCalPoint
   * \param sensorNumber Number of US sensor(beginn at 0 at the left of thr front bumber. Count clockwise)
   * \param x x-coordinates of calibration point in the car grid
   * \param y y-coordinates of calibration point in the car grid
   * \param d distance from calibration point to US sensor
   * \param calPointNumber Number of calibration Point. Two points are needed for each sensor
   */
  void setCalPoint(int sensorNumber, double x, double y, double d, int calPointNumber);
  /*!
   * \brief getValuesFromCons show consule Prompt for setting cal points
   */
  void getValuesFromCons(void);

private:

  /*! calcSensorPos Calculate Sensorpostions in the car. Writes to mUsSensorPos */
  void calcSensorPos();


  struct structCalPoint{
    double distance;
    double x;
    double y;
  };
  //!holds each calibration point loaded from the file
  structCalPoint mCalPoints[NUMBERSENSOR][NUMBERCALPOINTSPERSENSOR];


  /*!
   * \brief The structUsSensorPos struct holds the postion of the Sensor
   *
   * The position is stored in an speacial format so it is easy to calclulate the postion of the objects
   *
   * ______
   * |    /
   * |   /_______
   * |  /        |
   * | / angle    distanceOffset (look at the hypotenuse)
   * |/__________|
   * |           |
   * |            y
   * |___________|
   *
   */
  struct structUsSensorPos{
    double yOffset;
    double angle;
    double distanceOffset;
  };

  /*! mUsSensorPos holds the postion of the US Sensors */
  structUsSensorPos mUsSensorPos[NUMBERSENSOR];
  const double MAXDIST;
  const double MINDIST;
public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW


};

}	// namespace
}	// namespace

#endif // OADRIVE_OBSTACLE_PROCESSUS_H
