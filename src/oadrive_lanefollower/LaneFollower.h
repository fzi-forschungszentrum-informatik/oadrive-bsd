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
 * \date    2015-11-24
 *
 * Center-Piece for Oadrive, representtion of the current world.
 *
 */
//----------------------------------------------------------------------

#ifndef KACADU_LANEFOLLOWER_H
#define KACADU_LANEFOLLOWER_H

#include <oadrive_control/LateralController.h>
#include <oadrive_core/Types.h>
#include <oadrive_core/Trajectory2d.h>
#include <oadrive_core/Interpolator.h>
#include <oadrive_util/CoordinateConverter.h>

#include <opencv2/imgproc/imgproc.hpp>


#define HAARFILTER
//#define NEURALNETS

#ifdef HAARFILTER
	//#include "HaarLaneTracker/HaarFilter.h"
	#include <oadrive_lanedetection/HaarFilter.h>
#else
	#include "NNLaneTracker/NNPredictor.h"
#endif

class LaneFollower
{
	public:
		LaneFollower();
		~LaneFollower() {};
		
		void setImage( cv::Mat image );
		void setPose( float x, float y, float yaw );

		cv::Mat getResultImage();

		void createTestTrajectory();
		float steer();

		oadrive::core::Trajectory2d getTrajectory() { return mTrajectory; }

		void setConfigFile( std::string file );

	private:

		oadrive::core::ExtendedPose2d mPose;

		oadrive::core::Trajectory2d mTrajectory;
		oadrive::control::LateralController mLateralController;

#ifdef HAARFILTER
		oadrive::lanedetection::HaarFilter mHaarFilter;
#else
		NNPredictor mNNPredictor;
#endif

		oadrive::util::CoordinateConverter mCoordConverter;
};



#endif
