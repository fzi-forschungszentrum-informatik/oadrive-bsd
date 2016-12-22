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
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2015-11-24
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_UTIL_BIRDVIEWCONVERTER_H
#define OADRIVE_UTIL_BIRDVIEWCONVERTER_H
#include <opencv2/core/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
//#include <opencv2/imgcodecs.hpp>

#define DEPTH_IMAGE_HEIGHT 240
#define DEPTH_IMAGE_WIDTH 320
namespace oadrive {
namespace util {
/*!
   \brief The BirdViewConverter class
   This class converts the image into a Birdview image via the warp perspective. (It is speed up via remap)
   You must provide a special camera calibration file which contains a warp matrix (This is not the matrix wich is provided by the normal opencv camera calibration) This file can be generated with
    the test test_oadrive_birdViewCal.cpp or the class CameraCalibration
 */
class BirdViewConverter
{
public:
  BirdViewConverter();
  /*!
     * \brief transform undisort image and transform image into Birdview. Configfile must be read first
     * \param image Image to Transform
     * \return Birdview Image
     */
  cv::Mat transform(cv::Mat image);
  /*!
    * \brief loadConfig Read Calibration File with WarpMatrix
    * \param path Path to Calibration File (Hint: Warp Matrix ist not the camera calibration from OPENCV)
    * \return true if reading was successfull
    */
  bool loadConfig(std::string path);

  //! return distortion coefficients which were read from config file
  cv::Mat getDistortionCoefficients();

  //! return camera matrix which was read from config file
  cv::Mat getCameraMatrix();

private:
  //!Enable undistort during transform
  bool mUndistEnable;
  //!Matrix to transform the Image
  cv::Mat mWarpMatrix;
  //!Matrix to transform depth image
  cv::Mat mWarpDepthMatrix;
  //! Matrix to distort image
  cv::Mat mCameraMatrix, mDistCoeffs;
  //! Precalculated distortion and transformation map
  cv::Mat undistortAndWarpXMapInt, undistortAndWarpYMapInt;

  //! size of the picture
  cv::Size mImgSize;

  //! Path to the complete calibration FIle
  std::string mCompleteCalFile;
};
}
}
#endif // OADRIVE_UTIL_BIRDVIEWCONVERTER_H
