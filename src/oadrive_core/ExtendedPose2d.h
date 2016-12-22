// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// This program is free software licensed under the CDDL
// (COMMON DEVELOPMENT AND DISTRIBUTION LICENSE Version 1.0).
// You can find a copy of this license in LICENSE in the top
// directory of the source code.
//
// © Copyright 2016 FZI Forschungszentrum Informatik, Karlsruhe, Germany
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Sebastian Klemm <klemm@fzi.de>
 * \date    2014-03-07
 *
 */
//----------------------------------------------------------------------
#ifndef OADRIVE_CORE_EXTENDED_POSE_2D_H_INCLUDED
#define OADRIVE_CORE_EXTENDED_POSE_2D_H_INCLUDED

#include <ostream>
#include <cmath>
#include <limits>
#include <Eigen/StdVector>
#include <oadrive_core/Types.h>
#include <oadrive_core/Pose.h>
#include <icl_core_logging/ThreadStream.h>

#include <boost/shared_ptr.hpp>
#include <map>

namespace oadrive {
namespace core {

/*! A Pose type for navigation in SE2.
 *
 *  Combines a Pose2d and extends it with some additional meta data.
 *  Provides some convenience functions for
 *  quick data access / conversion.
 *
 *  The MetaPose is intended to be used in a kinodynamic context,
 *  e.g. when description of how a moving object is positioned in 2D
 *  space.
 */
class ExtendedPose2d
{
public:

  //! Constructor
  ExtendedPose2d();

  //! Constructor from Pose2d
  ExtendedPose2d(const Pose2d& pose);

  //! Constructor from x, y and yaw
  ExtendedPose2d(double x, double y, double yaw);

  //! Destructor
  ~ExtendedPose2d();

  // ----- read access -----

  // single components
  double getX() const;
  double getY() const;
  double getYaw() const;
  double getCurvature() const;
  double getVelocity() const;

  // multiple components
  Position2d getPosition() const;
  Eigen::Matrix2d getOrientation() const;

  // the whole pose
  const Pose2d& pose() const;
  Pose2d getPose() const;

  // ----- write access -----

  // single components
  void setX(double x);
  void setY(double y);
  void setYaw(double yaw);
  void setCurvature(double curvature);

  /*! Set the (signed) velocity.
   *  A positive velocity will be considered 
   *  movement along pose orientation, 
   *  and a negative the opposite direction.
   */
  void setVelocity(double velocity);

  // multiple components
  void setPosition(double x, double y);
  void setPosition(const Position2d& position);
  void setOrientation(double yaw);
  void setOrientation(const Eigen::Matrix2d& rotation_matrix);

  // the whole pose
  Pose2d& pose();
  void setPose(double x, double y, double yaw);
  void setPose(const Pose2d& pose);


  void print(std::ostream& out) const;
  void print(std::ostream& out, const std::string& comment) const;

  /*! Direct the pose's orientation to the
   *  destination position
   */
  void directTo(const Position2d& dest_position);

  /*! Calculate the translational and rotational distance.
   *  Each of the three components has the weight 1/3.
   */
  double distance(const ExtendedPose2d& other) const;

  friend ExtendedPose2d operator+(const ExtendedPose2d &pose, const Position2d &pos);
  friend ExtendedPose2d operator-(const ExtendedPose2d &pose, const Position2d &pos);
  
private:

  //! The raw pose information.
  Pose2d m_pose;

  /*! The 2D curvature at this pose. The curvature is signed, 
   *  and the sign is interpreted as follows:
   *  \li A negative curvature indicates a right turn.
   *  \li A positive curvature indicates a left turn.
   */
  double m_curvature;

  /*! The absolute velocity an agent or object has at this pose in m/s.
   *  The velocity may only be >=0.
   *  The movement direction is stored in \see m_movement_direction.
   */
  double m_velocity;

public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

std::ostream& operator << (std::ostream &os, const ExtendedPose2d &pose);
std::istream& operator >> (std::istream &is, ExtendedPose2d &pose);
icl_core::logging::ThreadStream& operator << (icl_core::logging::ThreadStream &os,
		const ExtendedPose2d &pose);

// some more typedefs
typedef std::vector<ExtendedPose2d, Eigen::aligned_allocator<ExtendedPose2d> > ExtendedPose2dVector;
typedef boost::shared_ptr< ExtendedPose2dVector > ExtendedPose2dVectorPtr;

//! \todo this should be correct but doesn't work:
//typedef std::map <unsigned int , ExtendedPose2dVector, Eigen::aligned_allocator<std::pair<unsigned int , ExtendedPose2dVector> > > ExtendedPose2dVectorByUIntMap;

typedef std::map <unsigned int , ExtendedPose2dVector> ExtendedPose2dVectorByUIntMap;

} // end of ns
} // end of ns

#endif
