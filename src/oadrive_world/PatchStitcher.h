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
 * \date    2015-12-24
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_WORLD_PATCHSTITCHER_H
#define OADRIVE_WORLD_PATCHSTITCHER_H

#include "Patch.h"
#include "PatchStitcher.h"

namespace oadrive{
namespace world{

class PatchStitcher
{
public:
  PatchStitcher();
  virtual ~PatchStitcher() {};
  bool addStreetPatch( PatchPtr streetPatch );

private:

  /*! Attempt to merge the new patch into the list of patches.
                 * The new patch is merged if another patch is found which is similar (position and type).
                 * \return true if new patch was "absorbed" (merged) into the list, false otherwise. */
  bool mergeIntoList( PatchPtr newPatch );
  PatchPtr findReferencePatchFor( PatchPtr patch );
  /*! Returns true if the patch overlaps with any CROSS_SECTIONs. */
  bool overlapsCrossSection( PatchPtr patch );
  
  /* Find out if the given Patch is too close to a CROSS_SECTION: */
  bool tooCloseToCrossSection( PatchPtr patch, float dist );

  float mMaximumAngle;
  float mMinimumDistBetweenCrossings;

public:
  // use a proper alignment when calling the constructor.
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}	// namespace
}	// namespace

#endif  // OADRIVE_WORLD_PATCHSTITCHER_H
