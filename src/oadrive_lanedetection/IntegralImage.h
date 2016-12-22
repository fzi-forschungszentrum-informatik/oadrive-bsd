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
 * \author  Jan-Markus Gomer <uecxl@student.kit.edu>
 * \date    2015-12-06
 *
 */
//----------------------------------------------------------------------

#ifndef INTEGRALIMAGE_H_
#define INTEGRALIMAGE_H_

#include <opencv2/imgproc/imgproc.hpp>

class IntegralImage {
public:
  void setIntegralImage(cv::Mat& integralImage);
  int boxSum(cv::Point2i position, cv::Point2i delta);
  virtual int boxSum(cv::Point2i (&points)[4]);
  virtual int boxSum(int positionY, int positionX, int deltaY, int deltaX);
  virtual cv::Point2i getPointWithOffset(cv::Point2i& vectorMiddleOfFeature, cv::Point2i& vectorToRotationPoint);
  virtual cv::Point2i rotatePoint(cv::Point2i& vectorToBeRotated);

protected:
  cv::Mat integralImage;
};

class IntegralImageRotated : public IntegralImage {
public:
  int boxSum(cv::Point2i (&points)[4]);
  int boxSum(int positionY, int positionX, int deltaY, int deltaX);
  cv::Point2i getPointWithOffset(cv::Point2i& vectorMiddleOfFeature, cv::Point2i& vectorToRotationPoint);
  cv::Point2i rotatePoint(cv::Point2i& vectorToBeRotated);

private:
  static const float angle = 0.7071067811865475244008443621048490392848359376884740; // cos(Pi/4)=sin(Pi/4)

};

#endif /* INTEGRALIMAGE_H_ */
