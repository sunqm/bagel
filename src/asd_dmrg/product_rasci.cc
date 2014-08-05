//
// BAGEL - Parallel electron correlation program.
// Filename: asd_dmrg/product_rasci.cc
// Copyright (C) 2014 Toru Shiozaki
//
// Author: Shane Parker <shane.parker@u.northwestern.edu>
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

#include <src/asd_dmrg/product_rasci.h>
#include <src/asd_dmrg/form_sigma.h>
#include <src/math/davidson.h>

using namespace std;
using namespace bagel;

ProductRASCI::ProductRASCI(shared_ptr<const PTree> input, shared_ptr<const Reference> ref, shared_ptr<const DMRG_Block> left)
 : input_(input), ref_(ref), left_(left)
{
  print_header();

  // at the moment I only care about C1 symmetry, with dynamics in mind
  if (ref_->geom()->nirrep() > 1) throw runtime_error("RASCI: C1 only at the moment.");

  max_iter_ = input_->get<int>("maxiter", 100);
  davidson_subspace_ = input_->get<int>("davidson_subspace", 20);
  thresh_ = input_->get<double>("thresh", 1.0e-8);
  print_thresh_ = input_->get<double>("print_thresh", 0.05);

  batchsize_ = input_->get<int>("batchsize", 512);

  nstates_ = input_->get<int>("nstates", 1);
  nguess_ = input_->get<int>("nguess", nstates_);

  // Set up wavefunction parameters for site
  // No defaults for RAS, must set "active"
  const shared_ptr<const PTree> iactive = input_->get_child("active");
  if (iactive->size() != 3) throw runtime_error("Must specify three active spaces in RAS calculations.");
  vector<set<int>> acts;
  for (auto& i : *iactive) {
    set<int> tmpset;
    for (auto& j : *i)
      if (!tmpset.insert(lexical_cast<int>(j->data()) - 1).second) throw runtime_error("Duplicate orbital in list of active orbitals.");
    acts.push_back(tmpset);
  }
  ref_ = ref_->set_ractive(acts[0], acts[1], acts[2]);
  ncore_ = ref_->nclosed();

  ras_ = {{ static_cast<int>(acts[0].size()), static_cast<int>(acts[1].size()), static_cast<int>(acts[2].size()) }};
  norb_ = ras_[0] + ras_[1] + ras_[2];

  max_holes_ = input_->get<int>("max_holes", 0);
  max_particles_ = input_->get<int>("max_particles", 0);

  // Set up wavefunction parameters for whole system
  // additional charge
  const int charge = input_->get<int>("charge", 0);

  // nspin is #unpaired electron 0:singlet, 1:doublet, 2:triplet, ... (i.e., Molpro convention).
  const int nspin = input_->get<int>("nspin", 0);
  if ((ref_->geom()->nele()+nspin-charge) % 2 != 0) throw runtime_error("Invalid nspin specified");
  nelea_ = (ref_->geom()->nele()+nspin-charge)/2 - ncore_;
  neleb_ = (ref_->geom()->nele()-nspin-charge)/2 - ncore_;

  // TODO allow for zero electron (quick return)
  if (nelea_ < 0 || neleb_ < 0) throw runtime_error("#electrons cannot be negative in ProductRASCI");

  energy_.resize(nstates_);

  // construct a space of several RASDeterminants to hold all of the RAS information
  space_ = make_shared<RASSpace>(ras_, max_holes_, max_particles_);

  // compute integrals
  auto coeff = make_shared<Matrix>(ref_->geom()->nbasis(), ref_->nclosed() + ref_->nact() + left_->norb());
  coeff->copy_block(0, 0, ref_->geom()->nbasis(), ref_->nclosed() + ref_->nact(), ref_->coeff()->slice(0, ref_->nclosed() + ref_->nact()));
  coeff->copy_block(0, ref_->nclosed() + ref_->nact(), ref_->geom()->nbasis(), left_->norb(), *left_->coeff());
  ref_ = make_shared<Reference>(ref_->geom(), std::make_shared<Coeff>(move(*coeff)), ref_->nclosed(), ref_->nact() + left_->norb(), 0);
  jop_ = make_shared<DimerJop>(ref_, ref_->nclosed(), ref_->nclosed() + norb_, ref_->nclosed() + ref_->nact(), ref_->coeff());

  construct_denom();
}

void ProductRASCI::print_header() const {
  cout << "  --------------------------------------" << endl;
  cout << "        ProductRAS-CI calculation" << endl;
  cout << "  --------------------------------------" << endl << endl;
}


void ProductRASCI::compute() {
  Timer pdebug(0);

  // Creating an initial CI vector
  vector<shared_ptr<ProductRASCivec>> cc(nstates_);
  generate(cc.begin(), cc.end(), [this] () { return make_shared<ProductRASCivec>(space_, left_, nelea_, neleb_); });

  // find determinants that have small diagonal energies
  model_guess(cc);

  pdebug.tick_print("guess generation");

  // nuclear energy retrieved from geometry
  const double nuc_core = ref_->geom()->nuclear_repulsion() + jop_->core_energy();

  // Davidson utility
  DavidsonDiag<ProductRASCivec> davidson(nstates_, davidson_subspace_);

  // Object in charge of forming sigma vector
  FormSigmaProdRAS form_sigma(batchsize_);

  // main iteration starts here
  cout << "  === ProductRAS-CI iterations ===" << endl << endl;
  vector<bool> converged(nstates_, false);

  for (int iter = 0; iter != max_iter_; ++iter) {
    Timer calctime;

    // form a sigma vector given cc
    vector<shared_ptr<ProductRASCivec>> sigma = form_sigma(cc, jop_, converged);
    pdebug.tick_print("sigma formation");

    // feeding sigma vectors into Davidson
    vector<shared_ptr<const ProductRASCivec>> ccn, sigman;
    for (int i = 0; i < nstates_; ++i) {
      if (!converged[i]) {
        ccn.push_back(make_shared<const ProductRASCivec>(*cc.at(i))); // copy in civec
        sigman.push_back(sigma.at(i)); // place in sigma
      }
      else {
        ccn.push_back(nullptr);
        sigman.push_back(nullptr);
      }
    }
    const vector<double> energies = davidson.compute(ccn, sigman);
    // get residual and new vectors
    vector<shared_ptr<ProductRASCivec>> errvec = davidson.residual();
    pdebug.tick_print("davidson");

    // compute errors
    vector<double> errors;
    for (int i = 0; i != nstates_; ++i) {
      errors.push_back(errvec.at(i)->rms());
      converged.at(i) = errors.at(i) < thresh_;
    }
    pdebug.tick_print("error");

    if (!all_of(converged.begin(), converged.end(), [] (const bool b) { return b; })) {
      // denominator scaling
      for (int ist = 0; ist != nstates_; ++ist) {
        if (converged[ist]) continue;
        for (auto& denomsector : denom_->sectors()) {
          auto dbegin = denomsector.second->begin();
          auto dend = denomsector.second->end();
          auto targ_iter = cc.at(ist)->sector(denomsector.first)->begin();
          auto source_iter = errvec.at(ist)->sector(denomsector.first)->begin();
          const double en = energies.at(ist);
          transform(dbegin, dend, source_iter, targ_iter, [&en] (const double den, const double cc) { return cc / min(en - den, -0.1); });
        }
        //cc.at(ist)->spin_decontaminate();
        cc.at(ist)->normalize();
        //cc.at(ist)->synchronize();
      }
    }

    pdebug.tick_print("denominator");

    // printing out
    if (nstates_ != 1 && iter > 0) cout << endl;
    for (int i = 0; i != nstates_; ++i) {
      cout << setw(7) << iter << setw(3) << i << setw(2) << (converged[i] ? "*" : " ")
                              << setw(17) << fixed << setprecision(8) << energies[i]+nuc_core << "   "
                              << setw(10) << scientific << setprecision(2) << errors[i] << fixed << setw(10) << setprecision(2)
                              << calctime.tick() << endl;
      energy_[i] = energies[i]+nuc_core;
    }
    if (all_of(converged.begin(), converged.end(), [] (const bool b) { return b; })) break;
  }
  // main iteration ends here

  cc_ = davidson.civec();

  if (all_of(converged.begin(), converged.end(), [] (const bool b) { return b; })) {
    cout << " ----- ProductRASCI calculation converged! -----" << endl;
    cout << " Final energies:" << endl;
    for (int i = 0; i < nstates_; ++i)
      cout << setw(7) << i << setw(17) << fixed << setprecision(8) << energy_[i] << " Hartree" << endl;
    cout << endl;
    if (nstates_ > 1) {
      cout << " Excitation energies (eV):" << endl;
      for (int i = 1; i < nstates_; ++i)
        cout << setw(7) << i << setw(17) << fixed << setprecision(8) << (energy_[i] - energy_.front()) * au2eV__ << " eV" << endl;
    }
  }
  else {
    cout << " WARNING: calculation failed to converge after " << max_iter_ << " iterations." << endl;
  }

  for (int istate = 0; istate < nstates_; ++istate) {
    cout << endl << "     * state " << setw(3) << istate << ", <S^2> = " << setw(6) << setprecision(4) << cc_.at(istate)->spin_expectation()
                 << ", E = " << setw(17) << fixed << setprecision(8) << energy_[istate] << endl;
    cc_.at(istate)->print(print_thresh_);
  }
}
