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
 * \author  David Zimmerer <dzimmerer@gmail.com>
 * \author  Peter Zimmer <peter-zimmer@gmx.net>
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2015-11-24
 *
 */
//----------------------------------------------------------------------


#ifndef OADRIVE_WORLD_ENVOBJECT_H
#define OADRIVE_WORLD_ENVOBJECT_H


#include <ctime>
#include <boost/shared_ptr.hpp>
#include <list>
//#include  <tgmath.h>

#include <oadrive_core/ExtendedPose2d.h>

namespace oadrive{
namespace world{

enum EnvObjType { PATCH=1, OBSTACLE=2, EVENT=3 };    

class EventRegion;	// predefine so shared_ptr can use it
typedef boost::shared_ptr<EventRegion> EventRegionPtr;		// define before class because it uses the PatchPtr
typedef std::list<EventRegionPtr> EventRegionPtrList;
class EnvObject;	// predefine so shared_ptr can use it
typedef boost::shared_ptr<EnvObject> EnvObjectPtr;		// define before class because it uses the PatchPtr
typedef std::list<boost::shared_ptr<EnvObject> > EnvObjectPtrList;

class EnvObject {

public:
  EnvObject(const oadrive::core::ExtendedPose2d &mPose, double mWidth = 0, double mHeight = 0);
  virtual ~EnvObject();

  oadrive::core::ExtendedPose2d getPose() const;
  void setPose(const oadrive::core::ExtendedPose2d &mPose);
  double getProbability() const;
  void setProbability(double probability);
  double getLikelyhood() const;
  void setLikelyhood(double likelyhood);
  int getType() const;
  void setType(int mType);

  double getX();
  void setX(double x);
  double getY();
  void setY(double y);
  double getYaw();
  void setYaw(double yaw);

  double getHeight() const;
  void setHeight(double mHeight);
  double getWidth() const;
  void setWidth(double mWidth);

  double getTime() const;
  void setTime(double mTime);
  void updateTime();

  unsigned int getId() const;

  double calcDistTo(EnvObjectPtr other);
  double calcDistTo( const oadrive::core::ExtendedPose2d &mPose );
  double calcDistTo(const core::Position2d &pos);

  double getTempDist();
  void calcTempDist(const oadrive::core::ExtendedPose2d &pose);

  bool contains(double x, double y);
  double calcAngleOffset(EnvObject* otherObject);
  double calcAngleOffset(const oadrive::core::ExtendedPose2d &mPose);

  bool isPointInside( const oadrive::core::ExtendedPose2d &M );
  //bool isPointInside( Eigen::Vector2d M );
  /*! Calculates the Corner Points of the patch and some scalar products..
   * These are used to find if a point is inside the rectangle or not.
   * \note MUST be called whenever the position or orientation of the patch changes! */
  void updateCorners();

  unsigned int checkIntersections( const oadrive::core::Position2d &start, const oadrive::core::Position2d &end );

  /*! Convert local coordinates pos to global coordinates. */
  oadrive::core::ExtendedPose2d toWorldCoords( const oadrive::core::ExtendedPose2d &pos );

  oadrive::core::Position2d getCornerA();
  oadrive::core::Position2d getCornerB();
  oadrive::core::Position2d getCornerC();
  oadrive::core::Position2d getCornerD();
  bool checkOverlap(EnvObjectPtr other);


  static unsigned int IDCounter;

  int getTimeSinceLastUpdate();
  
  void addEventRegion( EventRegionPtr evRegion );
  void clearEventRegions();
  EventRegionPtrList getEventRegions() { return mEventRegions; }
  void removeEventRegion(EventRegionPtr evRegion);

  /*! Convenience function for objects which only have one event region.
   * Returns first event region. */
  EventRegionPtr getEventRegion();

protected:
  int mType;

  EventRegionPtrList mEventRegions;

private:

  unsigned int mId;

  oadrive::core::ExtendedPose2d mPose;
  double mProbability;
  double mLikelyhood;
  double mTime;

  double mWidth, mHeight;
  double mTempDist;

  oadrive::core::Position2d mCornerA;
  oadrive::core::Position2d mCornerB;
  oadrive::core::Position2d mCornerC;
  oadrive::core::Position2d mCornerD;

  oadrive::core::Position2d mAB;
  oadrive::core::Position2d mAD;
  float mAB_AB;
  float mAD_AD;

  double crossProduct( const oadrive::core::Position2d &p1, const oadrive::core::Position2d &p2 );

  bool lineSegmentIntersection( const oadrive::core::Position2d &p,
                                const oadrive::core::Position2d &q,
                                const oadrive::core::Position2d &r,
                                const oadrive::core::Position2d &s );
public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

}	// namespace
}	// namespace

#endif /* OADRIVE_WORLD_ENVOBJECT_H */
