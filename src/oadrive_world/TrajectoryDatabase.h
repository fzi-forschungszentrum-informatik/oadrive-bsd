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
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2016-02-06
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_WORLD_TRAJECTORYDATABASE_H
#define OADRIVE_WORLD_TRAJECTORYDATABASE_H

#include <oadrive_core/Trajectory2d.h>
#include <map>
#include <string>
#include "MultiTrajectory.h"

namespace oadrive{
namespace world{

enum enumTrajectory { TRAJ_PULLOUT_PARALLEL=0, TRAJ_PULLOUT_CROSS_LEFT=1,
  TRAJ_PULLOUT_CROSS_RIGHT=2, TRAJ_PARKING_PARALLEL=3, TRAJ_PARKING_CROSS=4,
  TRAJ_BACKUP_OBSTACLE=5, TRAJ_FORWARD_SMALL=6, TRAJ_U_TURN=7 };

class TrajectoryDatabase
{
public:
  /*! Loads all trajectory .txt files in the given directory and stores them. */
  static void load( std::string directory );
  /*! Clears the internal map. */
  static void clear();

  /*! Returns a trajectory by the given name.
                 * \note If the trajectory doesn't exits, this throws an error. */
  static MultiTrajectory getTrajectory( std::string name );

  /* Convenience overload for getTrajectory: returns first trajectory from multi-trajectory */
  static oadrive::core::Trajectory2d getSingleTrajectory( std::string name );

  static bool hasTrajectory( std::string name );

private:
  TrajectoryDatabase();	// private constructor -> static class!

  //static std::map<std::string, oadrive::core::Trajectory2d> database;
  //! \todo FIX THESE according to Eigen Guidelines!
  static std::map<std::string, MultiTrajectory> database;
public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}		// namespace
}		// namespace

#endif
