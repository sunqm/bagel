//
// BAGEL - Parallel electron correlation program.
// Filename: gnaibatch.cc
// Copyright (C) 2012 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
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


#include <src/integral/rys/gnaibatch.h>

using namespace std;
using namespace bagel;


GNAIBatch::GNAIBatch(const array<shared_ptr<const Shell>,2>& _info, const shared_ptr<const Molecule> mol, const tuple<int,int> i, shared_ptr<StackMem> stack)
  :  CoulombBatch_base(_info, mol, 1, stack), iatom_(i) {
  if (swap01_) {
    swap(get<0>(iatom_), get<1>(iatom_));
  }
  set_exponents();
  const double integral_thresh = PRIM_SCREEN_THRESH;

  this->allocate_arrays(primsize_*natom_);
  compute_ssss(integral_thresh);
  root_weight(primsize_*natom_);
}


void GNAIBatch::set_exponents() {
  exponents_ = unique_ptr<double[]>(new double[primsize_*2]);
  assert(primsize_ == basisinfo_[0]->exponents().size() * basisinfo_[1]->exponents().size());
  double* tmp = exponents_.get();
  for (auto i0 = basisinfo_[0]->exponents().begin(); i0 != basisinfo_[0]->exponents().end(); ++i0) {
    for (auto i1 = basisinfo_[1]->exponents().begin(); i1 != basisinfo_[1]->exponents().end(); ++i1, tmp+=2) {
      tmp[0] = *i0;
      tmp[1] = *i1;
    }
  }
}


shared_ptr<GradFile> GNAIBatch::compute_gradient(shared_ptr<const Matrix> d, const int iatom0, const int iatom1, const int natom) const {
  auto out = make_shared<GradFile>(natom);
  for (int l = 0; l != natom; ++l)
    for (int k = 0; k != 3; ++k)
      out->element(k, l) += blas::dot_product(d->data(), d->size(), data_+size_block_*(k+3*l));
  return out;
}


void GNAIBatch::root_weight(const int ps) {
  if (amax_ + cmax_ == 0) {
    for (int j = 0; j != screening_size_; ++j) {
      int i = screening_[j];
      if (std::abs(T_[i]) < T_thresh__) {
        weights_[i] = 1.0;
      } else {
        const double sqrtt = std::sqrt(T_[i]);
        const double erfsqt = inline_erf(sqrtt);
        weights_[i] = erfsqt * std::sqrt(pi__) * 0.5 / sqrtt;
      }
    }
  } else {
    eriroot__.root(rank_, T_, roots_, weights_, ps);
  }
}
