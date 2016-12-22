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
 * \date    2015-11-01
 *
 */
//----------------------------------------------------------------------

/*! Defines types and enums needed for street patch detection. */

#ifndef OADRIVE_LANEDETECTION_STREETTYPES_H
#define OADRIVE_LANEDETECTION_STREETTYPES_H

#include <map>
#include <list>
#include <oadrive_core/ExtendedPose2d.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace oadrive {
namespace lanedetection {

//const float STREET_FIELD_WIDTH = 1.0;
//const float STREET_WIDTH = 0.974;
//const float STREET_SIDE_TO_CENTER = 0.440 + 0.015 + 0.010;
//const float PATCH_LENGTH = 0.5;
//const float LANE_WIDTH = 0.440;

enum FeatureType { CENTER_LINE=0, SIDE_LINE=1, CROSSROAD_LINE=2, PARKING_LOT=3, CROSSROAD_CORNER=4, PARKING_LINE=5 };
const int numFeatureTypes = 6;

/*! A feature is found in images by the HaarFilter class. It represent a point where a 
 * certain kind of line (Side-line, center-line etc. was found). */
struct Feature
{
  FeatureType type;
  oadrive::core::ExtendedPose2d pose;
  oadrive::core::ExtendedPose2d worldPose;
  float probability;
  float historicWeight;
  unsigned int frameCounter;
  unsigned int maxNumberOfFrames;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
};

typedef std::vector<Feature, Eigen::aligned_allocator<Feature> > FeatureVector;

//enum PatchType { STRAIGHT=0 };
//const int numPatchTypes = 1;

/*! A Patch is a part of the street. Depending on the type, a patch can have different sizes, but it
 * is always represented by center position, direction and type. The dimensions/sizes (i.e. how to
 * draw the patch on the street) can be derived from the type and the center position. */
/*struct Patch
{
        PatchType type;
        oadrive::core::ExtendedPose2d pose;
        float probability;
};*/

/*! Features and patches can vote for patches in a "Hough" like space. (Inspired by the generalized
 * Hough Transform). Beforehand, Votes are defined for every kind of patch. For example, a center
 * line will have a vote for a straight patch which is at the position where the center line is.
 * Higher weights for votes will give the voting feature/patch more importance. For example, a
 * Corner feature or T-Feature could be considered "more important" because it contains a lot of
 * information, so the weights of this feature's Vote could be higher. */
struct Vote
{
  cv::Point2f pos;
  cv::Size size;
  unsigned int weight;
  float angleOffset;
  float historySizeInMeters;

  // Only needed for votes for ParkingLots:
  unsigned int separationID;
};

//extern std::map <PatchType , std::list <oadrive::core::ExtendedPose2d>  > RL; 
//RL[STRAIGHT] = std::list <ExtendedPose2d>;

}
}

#endif
