//
// Newint - Parallel electron correlation program.
// Filename: dipolebatch.cc
// Copyright (C) 2012 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the Newint package (to be renamed).
//
// The Newint package is free software; you can redistribute it and\/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// The Newint package is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the Newint package; see COPYING.  If not, write to
// the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <src/stackmem.h>
#include <src/osint/dipolebatch.h>
#include <src/rysint/carsphlist.h>
#include <src/rysint/hrrlist.h>

extern StackMem* stack;
static HRRList hrr;

using namespace std;

DipoleBatch::DipoleBatch(const vector<std::shared_ptr<Shell> >& _basis, const vector<double>& c)
 : OSInt(_basis, -1), center_(c) {

}  


DipoleBatch::~DipoleBatch() {
}


void DipoleBatch::compute() {
  double* stack_save = stack->get(0);

  double* intermediate_p = stack->get(prim0_*prim1_*asize_*3);
  perform_VRR(intermediate_p);

  double* cdata = data_;
  double* csource = intermediate_p;
  for (int iblock = 0; iblock != 3; ++iblock,
                                    cdata += size_block_, csource += prim0_*prim1_*asize_) {

    double* intermediate_c = stack->get(cont0_*cont1_*asize_);
    fill(intermediate_c, intermediate_c + cont0_*cont1_*asize_, 0.0);
    perform_contraction(asize_, csource, prim0_, prim1_, intermediate_c, 
                        basisinfo_[0]->contractions(), basisinfo_[0]->contraction_ranges(), cont0_, 
                        basisinfo_[1]->contractions(), basisinfo_[1]->contraction_ranges(), cont1_);

    double* intermediate_fi = stack->get(cont0_*cont1_*asize_intermediate_);

    if (basisinfo_[1]->angular_number() != 0) { 
      const int hrr_index = basisinfo_[0]->angular_number() * ANG_HRR_END + basisinfo_[1]->angular_number();
      hrr.hrrfunc_call(hrr_index, cont0_*cont1_, intermediate_c, AB_, intermediate_fi);
    } else {
      const unsigned int array_size = cont0_*cont1_*asize_intermediate_;
      copy(intermediate_c, intermediate_c+array_size, intermediate_fi);
    }

    if (spherical_) {
      struct CarSphList carsphlist;
      double* intermediate_i = stack->get(cont0_*cont1_*asize_final_);
      const unsigned int carsph_index = basisinfo_[0]->angular_number() * ANG_HRR_END + basisinfo_[1]->angular_number();
      const int nloops = cont0_ * cont1_;
      carsphlist.carsphfunc_call(carsph_index, nloops, intermediate_fi, intermediate_i); 

      const unsigned int sort_index = basisinfo_[1]->angular_number() * ANG_HRR_END + basisinfo_[0]->angular_number();
      sort_.sortfunc_call(sort_index, cdata, intermediate_i, cont1_, cont0_, 1, swap01_);
      stack->release(cont0_*cont1_*asize_final_);
    } else {
      const unsigned int sort_index = basisinfo_[1]->angular_number() * ANG_HRR_END + basisinfo_[0]->angular_number();
      sort_.sortfunc_call(sort_index, cdata, intermediate_fi, cont1_, cont0_, 1, swap01_);
    }

    stack->release(cont0_*cont1_*asize_intermediate_ + cont0_*cont1_*asize_);
  }

  stack->release(prim0_*prim1_*asize_*3);
  assert(stack_save == stack->get(0));
}


void DipoleBatch::perform_VRR(double* intermediate) {
  const int worksize = amax1_+1;
  double* workx = stack->get(worksize);
  double* worky = stack->get(worksize);
  double* workz = stack->get(worksize);
  double* workxd = stack->get(worksize);
  double* workyd = stack->get(worksize);
  double* workzd = stack->get(worksize);

  const double acx = basisinfo_[0]->position(0) - center_[0];
  const double acy = basisinfo_[0]->position(1) - center_[1];
  const double acz = basisinfo_[0]->position(2) - center_[2];

  const size_t size_block = prim0_*prim1_*asize_;

  // Perform VRR
  for (int ii = 0; ii != prim0_*prim1_; ++ii) {
    const int offset_ii = ii * asize_;
    double* current_data0 = intermediate + offset_ii;
    double* current_data1 = intermediate + offset_ii + size_block;
    double* current_data2 = intermediate + offset_ii + size_block*2;

    /// Sx(0 : i + j, 0) etc will be made here 
    workx[0] = coeffsx_[ii]; 
    worky[0] = coeffsy_[ii];
    workz[0] = coeffsz_[ii];
    workx[1] = (p_[ii * 3    ] - basisinfo_[0]->position(0)) * workx[0];
    worky[1] = (p_[ii * 3 + 1] - basisinfo_[0]->position(1)) * worky[0];
    workz[1] = (p_[ii * 3 + 2] - basisinfo_[0]->position(2)) * workz[0];
    for (int i = 2; i != amax1_+1; ++i) {
      workx[i] = (p_[ii * 3    ] - basisinfo_[0]->position(0)) * workx[i - 1] + 0.5 * (i - 1) / xp_[ii] * workx[i - 2]; 
      worky[i] = (p_[ii * 3 + 1] - basisinfo_[0]->position(1)) * worky[i - 1] + 0.5 * (i - 1) / xp_[ii] * worky[i - 2]; 
      workz[i] = (p_[ii * 3 + 2] - basisinfo_[0]->position(2)) * workz[i - 1] + 0.5 * (i - 1) / xp_[ii] * workz[i - 2]; 
    }

    // take a linear combination to get dipole..
    for (int i = 0; i != amax1_; ++i) {
      workxd[i] = workx[i+1] + acx*workx[i];
      workyd[i] = worky[i+1] + acy*worky[i];
      workzd[i] = workz[i+1] + acz*workz[i];
    }

    /// assembly process

    for (int iz = 0; iz <= amax_; ++iz) { 
      for (int iy = 0; iy <= amax_ - iz; ++iy) { 
        for (int ix = max(0, amin_ - iy - iz); ix <= amax_ - iy - iz; ++ix) { 
          int pos = amapping_[ix + amax1_ * (iy + amax1_ * iz)];
          current_data0[pos] = workxd[ix] * worky[iy] * workz[iz];
          current_data1[pos] = workx[ix] * workyd[iy] * workz[iz];
          current_data2[pos] = workx[ix] * worky[iy] * workzd[iz];
        }
      }
    }

  } // end of primsize loop

  stack->release(worksize*6);
}

