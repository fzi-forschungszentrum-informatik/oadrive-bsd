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

#include "IntegralImage.h"
#include <iostream>

using namespace std;
using namespace cv;

void IntegralImage::setIntegralImage(Mat& integralImage) {
  this->integralImage = integralImage;
}

int IntegralImage::boxSum(Point2i position, Point2i delta) {
  return boxSum(position.y, position.x, delta.y, delta.x);
}

int IntegralImage::boxSum(int positionY, int positionX, int deltaY, int deltaX) {
  return ( 	integralImage.at<int>(positionY - deltaY, positionX - deltaX)
                + integralImage.at<int>(positionY + deltaY, positionX + deltaX)
                - integralImage.at<int>(positionY + deltaY, positionX - deltaX)
                - integralImage.at<int>(positionY - deltaY, positionX + deltaX)
                );
}

Point2i IntegralImage::getPointWithOffset(Point2i& vectorMiddleOfFeature, Point2i& vectorToRotationPoint) {
  return vectorMiddleOfFeature + vectorToRotationPoint;
}

Point2i IntegralImage::rotatePoint(Point2i& vectorToBeRotated) {
  return vectorToBeRotated;
}

int IntegralImageRotated::boxSum(int positionY, int positionX, int deltaY, int deltaX) {
  Point2i middlePoint(positionX, positionY);

  Point2i vectorToP1(deltaX, -deltaY);
  Point2i vectorToP2(-deltaX, deltaY);
  Point2i vectorToP3(-deltaX, -deltaY);
  Point2i vectorToP4(deltaX, deltaY);

  Point2i rotatedVectorToP1 = rotatePoint(vectorToP1);
  Point2i rotatedVectorToP2 = rotatePoint(vectorToP2);
  Point2i rotatedVectorToP3 = rotatePoint(vectorToP3);
  Point2i rotatedVectorToP4 = rotatePoint(vectorToP4);

  return ( 	integralImage.at<int>(middlePoint + rotatedVectorToP1)
                + integralImage.at<int>(middlePoint + rotatedVectorToP2)
                - integralImage.at<int>(middlePoint + rotatedVectorToP3)
                - integralImage.at<int>(middlePoint + rotatedVectorToP4)
                );
}

int IntegralImage::boxSum(Point2i (&points)[4]) {
  return(		integralImage.at<int>(points[0])
      - integralImage.at<int>(points[2])
      + integralImage.at<int>(points[1])
      - integralImage.at<int>(points[3])
      );
}

int IntegralImageRotated::boxSum(Point2i (&points)[4]) {
  return(		integralImage.at<int>(points[0])
      - integralImage.at<int>(points[2])
      - integralImage.at<int>(points[3])
      + integralImage.at<int>(points[1])
      );
}

Point2i IntegralImageRotated::rotatePoint(Point2i& vectorToBeRotated) {
  Point2i vectorRotated;
  vectorRotated.x = round( angle * ( (float) vectorToBeRotated.x + (float) vectorToBeRotated.y ));
  vectorRotated.y = round( angle * ( - (float) vectorToBeRotated.x + (float) vectorToBeRotated.y ));

  return vectorRotated;
}

Point2i IntegralImageRotated::getPointWithOffset(Point2i& vectorMiddleOfFeature, Point2i& vectorToRotationPoint) {
  Point2i rotatedVectorToRotationPoint;
  rotatedVectorToRotationPoint = rotatePoint(vectorToRotationPoint);

  return vectorMiddleOfFeature + rotatedVectorToRotationPoint;
}
