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
 * \date    2015-11-08
 *
 */
//----------------------------------------------------------------------


#ifndef LINE_H_
#define LINE_H_

#include "HaarFilter.h"

#define MINIMUM_POINTS_FOR_SPLINE_APPROXIMATION 50
#define ALLOWED_PIXEL_DIFFERENCE_TO_PREVIOUS_DETECTION 25
#define RECALCULATE_SPLINE_AFTER_VALID_DETECTIONS 5
#define BORDER_LEFT_RIGHT 15

namespace oadrive{
namespace lanedetection{

class Line {
public:
	Line();
	virtual ~Line();
	void setPoint(int y, int x);
	int getPointX(int y);
	int getLastValidDetectionY();
	void setDetected();
	bool isDetected();
	int getPredictionX(int y);
	bool isDetectionValid(int y, int lowerRangeX = -1, int upperRangeX = -1);

	// TODO: shouldn't be public!
	void calculateSpline(int x1, int y1, int x2, int y2, int x3, int y3, cv::Vec3f& coefficients);
	int getSplineValue(int y, cv::Vec3f& coefficients);

private:
	cv::Vec<int, IMAGE_Y> points;
	cv::Vec3f splineCoefficients;
	int splineLastCalculatedY;
	bool detected;
	int lastValidDetectionY;

	bool recalculateSpline(int y);
	int getSplinePredictionX(int y);
	void approximateMissedDetections(int nextValidDetection);
	void setLastValidDetectionY(int y);
};
}	// namespace
}	// namespace

#endif /* LINE_H_ */
