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
 * \date    2015-11-01
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_OBSTACLE_DEPTHIMAGE_H
#define OADRIVE_OBSTACLE_DEPTHIMAGE_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include <oadrive_world/Environment.h>
namespace oadrive{
namespace obstacle{
/*!
   \brief The ProcessDepth class process the Depth image. (It substract a reference Image and after that it look for blobs. From this blobs it converts some pixel into objects. Look into get Objects findObjects)
 */
class ProcessDepth
{
public:
  //! \brief DepthImage
  //! \param refPath path to ref Image. (Must be an image without any obstacles)
  ProcessDepth(std::string refPath, util::CoordinateConverter *converter);

  //! \brief processImage processes the depthImage from segmenting to enviroment
  //! \param image depthImage
  void processDepthImageDebug(cv::Mat image);
  //! \brief getObjects starting point of the depth image processing
  //! \param image Depth image
  //! \return Objects in Car? cords
  ExtendedPose2dVectorPtr getObjects(cv::Mat image);

  /*!
     * \brief setRef set refernce Image
     * \param improve if True at each Row the median is calculated an set to the whole row
     */
  void setRef(cv::Mat, bool improve);
  /*!
    * \brief calcCleanImg extract objects based on a refernce picture (RefImage)
    * \param image Image to clean
    * \return clean Binary Image
    */
  cv::Mat calcCleanImg(cv::Mat image);
  /*!
     * \brief getRefImage get RefImage (also the Improved one for debugging)
     * \return
     */
  cv::Mat getRefImage() const;
  /*!
     * \brief findObjects applys closing to a binary image and search for objects larger than MIN_AREA
     * \param imageU binary Image
     * \param objects
     */
  void findObjects(cv::Mat imageU,cv::Mat original,std::vector<cv::Point2d> &objects,std::vector<int> &depth);

  void applyClosing(cv::Mat &image);
  /*!
     * \brief mergeObjectsToEnviroment check if Objects in Environment. If not add them
     * \param objects All Objects which should be in the Environment
     */
  //void mergeObjectsToEnviroment(std::vector<cv::Point2d> &objects);

  oadrive::util::CoordinateConverter *getPosConv() const;
  //! \brief setPosConv Set postionen Converter
  //! \param value
  void setPosConv(oadrive::util::CoordinateConverter *value);
  //! get debug image
  cv::Mat getDebugImage(cv::Mat image);

private:
  cv::Mat mRefImage;
  oadrive::util::CoordinateConverter *mPosConv;
  const double MIN_AREA;

  //! Size in meters above the real ground from where on obstacles are considered obstacles.
  float mReferenceImageThreshold;

  //! View distance in meters. Anything beyond this will be ignored.
  float mMaxViewDepth;

  void generateDepthReferenceImage();

  int mMaxDepthRow;
  int mHorizonRow;

public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW



};
}
}

#endif
