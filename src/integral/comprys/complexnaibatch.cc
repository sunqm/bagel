#if 1
//
// BAGEL - Parallel electron correlation program.
// Filename: complexnaibatch.cc
// Copyright (C) 2013 Toru Shiozaki
//
// Author: Ryan D. Reynolds <RyanDReynolds@u.northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// The BAGEL package is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the BAGEL package; see COPYING.  If not, write to
// the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <src/integral/comprys/complexnaibatch.h>
#include <src/integral/comprys/comperirootlist.h>
#include <complex>

using namespace std;
using namespace bagel;

const static CHRRList hrr;
const static CCarSphList carsphlist;

void ComplexNAIBatch::root_weight(const int ps) {
  assert(breit_ == 0);

  // standard 2-D interpolation
  if (any_of(T_, T_+ps, [](complex<double>k){return (k.imag() != 0.0);})
    || any_of(T_, T_+ps, [](complex<double>k){return (k.real() < 0.0);})) {
    complexeriroot__.root(rank_, T_, roots_, weights_, ps);

  // cheaper 1-D interpolation if T_ are all real and positive
  } else {
    const double num = rank_*ps;
    double* const T_temp = stack_->template get<double>(ps);
    double* const roots_temp = stack_->template get<double>(num);
    double* const weights_temp = stack_->template get<double>(num);

    for (int i=0; i!=ps; ++i) {
      T_temp[i] = T_[i].real();
    }
    eriroot__.root(rank_, T_temp, roots_temp, weights_temp, ps);
    for (int i=0; i!=num; ++i) {
      roots_[i] = roots_temp[i];
      weights_[i] = weights_temp[i];
    }

    stack_->release(num, weights_temp);
    stack_->release(num, roots_temp);
    stack_->release(ps, T_temp);
  }
}

std::complex<double> ComplexNAIBatch::get_PQ(const double coord1, const double coord2, const double exp1, const double exp2, const double one12,
                                             const int center1, const int dim, const bool swap) {
  const double Areal = coord1*exp1;
  const double Breal = coord2*exp2;
  const double Aimag = basisinfo_[center1]->vector_potential(dim);
  const double Bimag = basisinfo_[center1+1]->vector_potential(dim);
  double imag;
  if (swap) imag = 0.5*(Bimag - Aimag);
  else imag = 0.5*(Aimag - Bimag);
  const std::complex<double> num (Areal + Breal, imag);
  return num * one12;
}

void ComplexNAIBatch::compute() {
  const double zero = 0.0;

  complex<double>* const stack_save = stack_->template get<complex<double>>(size_alloc_);
  bkup_ = stack_save;

  const int worksize = rank_ * amax1_;

  complex<double>* const workx = stack_->template get<complex<double>>(worksize);
  complex<double>* const worky = stack_->template get<complex<double>>(worksize);
  complex<double>* const workz = stack_->template get<complex<double>>(worksize);

  const double Ax = basisinfo_[0]->position(0);
  const double Ay = basisinfo_[0]->position(1);
  const double Az = basisinfo_[0]->position(2);

  complex<double> r1x[20];
  complex<double> r1y[20];
  complex<double> r1z[20];
  complex<double> r2[20];

  const int alc = size_alloc_;
  std::fill_n(data_, alc, zero);

  const CSortList sort(spherical1_);

  // perform VRR
  const int natom_unit = natom_ / (2 * L_ + 1);
  assert(natom_ % (2 * L_ + 1) == 0);
  for (int xj = 0; xj != screening_size_; ++xj) {
    const int i = screening_[xj];
    const int iprim = i / natom_;
    const int resid = i % natom_;
    const int cell  = resid / natom_unit - L_;
    const int iatom = resid % natom_unit;
    double disp[3];
    disp[0] = disp[1] = 0.0;
    disp[2] = A_ * cell;
    const int offset_iprim = iprim * asize_;
    complex<double>* current_data = &data_[offset_iprim];

    const complex<double>* croots = roots_ + i * rank_;
    const complex<double>* cweights = &weights_[i * rank_];
    for (int r = 0; r != rank_; ++r) {
      r1x[r] = P_[i * 3    ] - Ax - (P_[i * 3    ] - mol_->atoms(iatom)->position(0) - disp[0]) * croots[r];
      r1y[r] = P_[i * 3 + 1] - Ay - (P_[i * 3 + 1] - mol_->atoms(iatom)->position(1) - disp[1]) * croots[r];
      r1z[r] = P_[i * 3 + 2] - Az - (P_[i * 3 + 2] - mol_->atoms(iatom)->position(2) - disp[2]) * croots[r];
      r2[r] = (1.0 - croots[r]) * 0.5 / xp_[i];

      workx[r] = cweights[r] * coeff_[i];
      worky[r] = 1.0;
      workz[r] = 1.0;
    }

    if (amax_ > 0) {
      for (int r = 0; r != rank_; ++r) {
        workx[rank_ + r] = r1x[r] * workx[r];
        worky[rank_ + r] = r1y[r] * worky[r];
        workz[rank_ + r] = r1z[r] * workz[r];
      }
      for (int j = 2; j <= amax_; ++j) {
        const int offset = j * rank_;
        for (int r = 0; r != rank_; ++r) {
          workx[offset + r] = r1x[r] * workx[offset - rank_ + r] + (j - 1.0) * r2[r] * workx[offset - rank_ * 2 + r];
          worky[offset + r] = r1y[r] * worky[offset - rank_ + r] + (j - 1.0) * r2[r] * worky[offset - rank_ * 2 + r];
          workz[offset + r] = r1z[r] * workz[offset - rank_ + r] + (j - 1.0) * r2[r] * workz[offset - rank_ * 2 + r];
        }
      }
    }
    // assembly step
    for (int iz = 0; iz <= amax_; ++iz) {
      for (int iy = 0; iy <= amax_ - iz; ++iy) {
        for (int ix = std::max(0, amin_ - iy - iz); ix <= amax_ - iy - iz; ++ix) {
          const int pos = amapping_[ix + amax1_ * (iy + amax1_ * iz)];
          for (int r = 0; r != rank_; ++r)
          current_data[pos] += workz[iz * rank_ + r] * worky[iy * rank_ + r] * workx[ix * rank_ + r];
        }
      }
    }
  }

  // contract indices 01
  // data will be stored in bkup_: cont01{ xyz{ } }
  {
    const int m = asize_;
    this->perform_contraction(m, data_, prim0size_, prim1size_, bkup_,
                        basisinfo_[0]->contractions(), basisinfo_[0]->contraction_ranges(), cont0size_,
                        basisinfo_[1]->contractions(), basisinfo_[1]->contraction_ranges(), cont1size_);
  }

  // HRR to indices 01
  // data will be stored in data_: cont01{ xyzab{ } }
  {
    if (basisinfo_[1]->angular_number() != 0) {
      const int hrr_index = basisinfo_[0]->angular_number() * ANG_HRR_END + basisinfo_[1]->angular_number();
      hrr.hrrfunc_call(hrr_index, contsize_, bkup_, AB_, data_);
    } else {
      std::copy(bkup_, bkup_+size_alloc_, data_);
    }
  }

  // Cartesian to spherical 01 if necesarry
  // data will be stored in bkup_
  if (spherical1_) {
    const int carsphindex = basisinfo_[0]->angular_number() * ANG_HRR_END + basisinfo_[1]->angular_number();
    const int nloops = contsize_;
    carsphlist.carsphfunc_call(carsphindex, nloops, data_, bkup_);
  }

  // Sort cont01 and xyzab
  // data will be stored in data_: cont1b{ cont0a{ } }
  if (spherical1_) {
    const unsigned int index = basisinfo_[1]->angular_number() * ANG_HRR_END + basisinfo_[0]->angular_number();
    sort.sortfunc_call(index, data_, bkup_, cont1size_, cont0size_, 1, swap01_);
  } else {
    const unsigned int index = basisinfo_[1]->angular_number() * ANG_HRR_END + basisinfo_[0]->angular_number();
    sort.sortfunc_call(index, bkup_, data_, cont1size_, cont0size_, 1, swap01_);
    std::copy(bkup_, bkup_+size_final_, data_);
  }

  stack_->release(worksize, workz);
  stack_->release(worksize, worky);
  stack_->release(worksize, workx);
  stack_->release(size_alloc_, stack_save);
}

#endif
