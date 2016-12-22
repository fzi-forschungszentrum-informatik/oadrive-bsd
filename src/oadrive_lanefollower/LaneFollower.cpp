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

#include "LaneFollower.h"

#include <iostream>

LaneFollower::LaneFollower()
	: mCoordConverter("/home/aadc/AADC/src/aadcUser/config/Goffin/BirdviewCal_lowRes.yml")
{
	createTestTrajectory();
    mLateralController.setTrajectory(mTrajectory);
}

void LaneFollower::setConfigFile( std::string file )
{
	mCoordConverter.readConfigFile( file );
}

void LaneFollower::setImage( cv::Mat image )
{
#ifdef HAARFILTER
	mHaarFilter.setImage( image );
	cv::Vec<int, IMAGE_Y> trajPoints;
	mHaarFilter.getLanePoints( trajPoints );

	oadrive::core::ExtendedPose2d pose;

	mTrajectory.clear();
	//oadrive::core::Trajectory2d trajectory;
	//pose = oadrive::core::ExtendedPose2d( 1.0*cos( mPose.getYaw()+M_PI ), 1.0*sin(mPose.getYaw()+M_PI), 0 );
	//mTrajectory.push_back( pose );
	mTrajectory.push_back( mPose );
	int lastX = image.size().width/2;
	int lastY = image.size().height/2;
	for( int i = trajPoints.rows-1; i >= 0; i-- )
	{
		if( trajPoints(i) > 0 )
		{
			//std::cout << "i: " << i << " " << trajPoints(i) << std::endl;
			//pose = mCoordConverter.getWorldCordFromBirdView( mPose, trajPoints(i), i );
			pose = mCoordConverter.pixel2World( mPose, cv::Point2f( trajPoints(i), i ) );
			//std::cout << "pose: " << pose.getX() << ", " << pose.getY() << std::endl;
			mTrajectory.push_back(pose);
			lastX = trajPoints(i);
			lastY = i;
		}
	}
	if( mTrajectory.size() < 21 )
	{
		for( int i = 0; i < 21; i++ )
		{
			//pose = mCoordConverter.getWorldCordFromBirdView( mPose, lastX, lastY - i*3 );
			pose = mCoordConverter.pixel2World( mPose, cv::Point2f( lastX, lastY - i*3 ) );
			mTrajectory.push_back(pose);
		}
	}
#else	// Neural Nets
	int x,y;
	mNNPredictor.getLanePoint( image, x, y);

	std::cout << "x,y " << x << " " << y << std::endl;

	oadrive::core::ExtendedPose2d endPose = mCoordConverter.pixel2World( mPose, cv::Point2f( x, y ) );

	mTrajectory.clear();
	//mTrajectory.push_back( mPose );

	float dx = endPose.getX() - mPose.getX();
	float dy = endPose.getY() - mPose.getY();

	float dist = std::sqrt( dx*dx + dy*dy );
	dx = dx/dist;
	dy = dy/dist;

	for( float i = 0; i < 2; i += 0.07 )
	{
		oadrive::core::ExtendedPose2d pose( mPose.getX() + i*dx, mPose.getY() + i*dy, 0 );
		mTrajectory.push_back( pose );
	}

#endif

	mTrajectory.calculateCurvature();
	//richtungenen der Posen untereinander
	mTrajectory.calculateOrientations();
	mLateralController.setTrajectory(mTrajectory);

	// Debug output:
	//mTrajectory.toGnuplot("/tmp/traj");
	/*oadrive::core::Trajectory2d t;
	t.push_back(mPose);
	t.toGnuplot("/tmp/t");*/
}

cv::Mat LaneFollower::getResultImage()
{
	#ifdef HAARFILTER
	return mHaarFilter.getResultImage();
	#else
	return mNNPredictor.getResultImage();
	#endif
}

void LaneFollower::setPose( float x, float y, float yaw )
{
	mPose = oadrive::core::ExtendedPose2d( x, y, yaw );
}

void LaneFollower::createTestTrajectory()
{
	oadrive::core::ExtendedPose2d p;

	float radius = 2.0;
	for(double x = -0.5; x < 0; x += 0.1 )
	{
		p.setPosition( x, 0.0 );
		mTrajectory.push_back(p);
	}
	for(double angle = 0; angle < M_PI*1.8; angle += 0.1 )
	{
		p.setPosition( radius*cos(angle-M_PI*0.5), radius + radius*sin(angle-M_PI*0.5 ) );
		mTrajectory.push_back(p);
	}
	//Kurvenradien berecheen
	mTrajectory.calculateCurvature();
	//richtungenen der Posen untereinander
	mTrajectory.calculateOrientations();
	//mTrajectory.toGnuplot("/tmp/our_traj");
}

float LaneFollower::steer()
{
	//steering berechnen
	float steering;
	steering =  mLateralController.calculateSteering( mPose.getPose() );

	/*std::cout<<"[SteeringTest] Yaw of the car "<<
	  oadrive::core::PoseTraits<oadrive::core::Pose2d>::yaw(mPose)<<std::endl;
	  std::cout<<"[SteeringTest] Trajectory Number "<< mLateralController.getIndexOfProjectedPose()<<std::endl;
	  std::cout<<"[SteeringTest] Yaw of the Pose "<< mTrajectory[mLateralController.getIndexOfProjectedPose()].getYaw()<<std::endl;*/

	std::cout << "steering: " << steering << " " << mPose.getYaw() << std::endl;

	return steering + 90.0;
}

