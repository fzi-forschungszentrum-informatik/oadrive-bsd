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
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2016-02-08
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_WORLD_ENVIRONMENTPAINTER_H
#define OADRIVE_WORLD_ENVIRONMENTPAINTER_H

#include <oadrive_core/Trajectory2d.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

#include <boost/date_time/local_time/local_time.hpp>

namespace oadrive{
namespace world{

class Environment;
/*! A class that can paint images of the Environment class.
 * This class is a friend of the Environment class, so it can access all of the Environment's
 * private members. */
class EnvironmentPainter
{
public:

  EnvironmentPainter();
  ~EnvironmentPainter() {};

  //! Draw the whole environment into an image.
  cv::Mat getEnvAsImage( Environment* env,
                                double x, double y, double radius, float pixelsPerMeter );

  static void drawTrajectory( cv::Mat &img, double x, double y,
                              double radius, float pixelsPerMeter, oadrive::core::Trajectory2d& traj,
                              cv::Scalar colStart, cv::Scalar colEnd );
  void drawPatches( Environment* env, cv::Mat &img, double x, double y,
                           double radius, float pixelsPerMeter );

  void drawTrafficSign( Environment* env, cv::Mat &img, double x, double y,
                               double radius, float pixelsPerMeter );
  void drawObstacles( Environment* env, cv::Mat &img, double x, double y,
                             double radius, float pixelsPerMeter );
  void drawEventRegions( Environment* env, cv::Mat &img, double x, double y,
                                double radius, float pixelsPerMeter );
  void drawHistory( Environment* env, cv::Mat &img, double x, double y,
                           double radius, float pixelsPerMeter );
  void drawGrid( Environment* env, cv::Mat &img, double x, double y,
                        double radius, float pixelsPerMeter, int imgSize );
  void drawCar( Environment* env, cv::Mat &img, double x, double y,
                       double radius, float pixelsPerMeter );
  void drawDebugPoints( Environment* env, cv::Mat &img, double x, double y,
                               double radius, float pixelsPerMeter );

  static std::string toJson(double radius);

private:
  bool mDrawEventRegions;
  bool mDrawHistory;
  bool mDrawObstacleDistances;

  float mCameraPosX;
  float mCameraPosY;
  float mCameraPosZ;
  float mCameraPitch;
public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

}	// namespace
}	// namespace

#endif  //OADRIVE_WORLD_ENVIRONMENTPAINTER_H
