//
// BAGEL - Parallel electron correlation program.
// Filename: ks.cc
// Copyright (C) 2013 Toru Shiozaki
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

#include <src/scf/ks/ks.h>
#include <src/scf/hf/fock.h>
#include <src/prop/multipole.h>
#include <src/util/math/diis.h>

using namespace std;
using namespace bagel;

void KS::compute() {

  Matrix intermediate = *tildex_ % *hcore_ * *tildex_;
  intermediate.diagonalize(eig());
  coeff_ = make_shared<Coeff>(*tildex_ * intermediate);
  shared_ptr<const Matrix> aodensity_ = coeff_->form_density_rhf(nocc_);

  cout << indent << "=== Nuclear Repulsion ===" << endl << indent << endl;
  cout << indent << fixed << setprecision(10) << setw(15) << geom_->nuclear_repulsion() << endl << endl;

  cout << indent << "     - DIIS with orbital gradients will be used." << endl << endl;
  cout << indent << "=== KS iteration (" << name_ << " / " << geom_->basisfile() << ") ===" << endl << indent << endl;

  DIIS<Matrix> diis(diis_size_);

  shared_ptr<Matrix> fock;

  Timer scftime;
  for (int iter = 0; iter != max_iter_; ++iter) {

    // fock operator without DFT xc
    fock = make_shared<Fock<1>>(geom_, hcore_, aodensity_, coeff_->slice(0, nocc_), false /*store*/, true /*rhf*/, func_->scale_ex());

    // add xc
    shared_ptr<const Matrix> xc;
    double exc;
    tie(xc, exc) = grid_->compute_xc(func_, coeff_->slice_copy(0, nocc_));

    energy_ = 0.5*((*hcore_+ *fock) * *aodensity_).trace() + exc + geom_->nuclear_repulsion();

    *fock += *xc;

    auto error_vector = make_shared<const Matrix>(*fock**aodensity_**overlap_ - *overlap_**aodensity_**fock);

    const double error = error_vector->rms();

    cout << indent << setw(5) << iter << setw(20) << fixed << setprecision(8) << energy_ << "   "
                                      << setw(17) << error << setw(15) << setprecision(2) << scftime.tick() << endl;

    if (error < thresh_scf_) {
      cout << indent << endl << indent << "  * SCF iteration converged." << endl << endl;
      break;
    } else if (iter == max_iter_-1) {
      cout << indent << endl << indent << "  * Max iteration reached in SCF." << endl << endl;
      break;
    }

    if (iter >= diis_start_)
      fock = diis.extrapolate({fock, error_vector});

    {
      auto intermediate = make_shared<Matrix>(*tildex_ % *fock * *tildex_);
      intermediate->diagonalize(eig());
      coeff_ = make_shared<const Coeff>(*tildex_**intermediate);
    }
    aodensity_ = coeff_->form_density_rhf(nocc_);

  }

  // by default we compute dipoles
  if (!geom_->external()) {
    Dipole mu(geom_, aodensity_);
    mu.compute();
  }
}


shared_ptr<const Reference> KS::conv_to_ref() const {
  auto out = make_shared<Reference>(geom_, coeff(), nocc(), 0, coeff()->mdim()-nocc(), energy());
  out->set_eig(eig_);
  return out;
}

