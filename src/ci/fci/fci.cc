//
// BAGEL - Parallel electron correlation program.
// Filename: fci.cc
// Copyright (C) 2011 Toru Shiozaki
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

#include <src/ci/fci/fci.h>
#include <src/ci/fci/space.h>
#include <src/ci/fci/modelci.h>
#include <src/util/combination.hpp>

using namespace std;
using namespace bagel;

BOOST_CLASS_EXPORT_IMPLEMENT(FCI)

FCI::FCI(std::shared_ptr<const PTree> idat, shared_ptr<const Geometry> g, shared_ptr<const Reference> r, const int ncore, const int norb, const int nstate)
 : Method(idat, g, r), ncore_(ncore), norb_(norb), nstate_(nstate), restarted_(false) {
  common_init();
}


void FCI::common_init() {
  print_header();

  const bool frozen = idata_->get<bool>("frozen", false);
  max_iter_ = idata_->get<int>("maxiter", 100);
  max_iter_ = idata_->get<int>("maxiter_fci", max_iter_);
  davidson_subspace_ = idata_->get<int>("davidson_subspace", 20);
  thresh_ = idata_->get<double>("thresh", 1.0e-10);
  thresh_ = idata_->get<double>("thresh_fci", thresh_);
  print_thresh_ = idata_->get<double>("print_thresh", 0.05);
  restart_ = idata_->get<bool>("restart", false);

  if (nstate_ < 0) nstate_ = idata_->get<int>("nstate", 1);
  nguess_ = idata_->get<int>("nguess", nstate_);

  const shared_ptr<const PTree> iactive = idata_->get_child_optional("active");
  if (iactive) {
    set<int> tmp;
    // Subtracting one so that orbitals are input in 1-based format but are stored in C format (0-based)
    for (auto& i : *iactive) tmp.insert(lexical_cast<int>(i->data()) - 1);
    ref_ = ref_->set_active(tmp);
    ncore_ = ref_->nclosed();
    norb_ = ref_->nact();
  }
  else {
    if (ncore_ < 0) ncore_ = idata_->get<int>("ncore", (frozen ? geom_->num_count_ncore_only()/2 : 0));
    if (norb_  < 0) norb_ = idata_->get<int>("norb", ref_->coeff()->mdim()-ncore_);
  }

  // Configure properties to be calculated on the final wavefunctions
  if (idata_->get<bool>("dipoles", false))
    properties_.push_back(make_shared<CIDipole>(ref_, ncore_, ncore_+norb_));

  // additional charge
  const int charge = idata_->get<int>("charge", 0);

  // nspin is #unpaired electron 0:singlet, 1:doublet, 2:triplet, ... (i.e., Molpro convention).
  const int nspin = idata_->get<int>("nspin", 0);
  if ((geom_->nele()+nspin-charge) % 2 != 0) throw runtime_error("Invalid nspin specified");
  nelea_ = (geom_->nele()+nspin-charge)/2 - ncore_;
  neleb_ = (geom_->nele()-nspin-charge)/2 - ncore_;

  // TODO allow for zero electron (quick return)
  // neleb can be = 0, so long as nelea > 0
  if (nelea_ <= 0 || neleb_ < 0) throw runtime_error("#electrons cannot be zero/negative in FCI");
  for (int i = 0; i != nstate_; ++i) weight_.push_back(1.0/static_cast<double>(nstate_));

  // resizing rdm vectors (with null pointers)
  rdm1_.resize(nstate_);
  rdm2_.resize(nstate_);
  energy_.resize(nstate_);

  // construct a determinant space in which this FCI will be performed.
  det_ = make_shared<const Determinants>(norb_, nelea_, neleb_);
}


void FCI::model_guess(shared_ptr<Dvec> out) {
  multimap<double, pair<bitset<nbit__>, bitset<nbit__>>> ordered_elements;
  const double* d = denom_->data();
  for (auto& abit : det_->string_bits_a()) {
    for (auto& bbit : det_->string_bits_b()) {
      ordered_elements.emplace(*d++, make_pair(abit, bbit));
    }
  }

  vector<pair<bitset<nbit__>, bitset<nbit__>>> basis;
  double last_value = 0.0;
  for (auto& p : ordered_elements) {
    double val = p.first;
    if (basis.size() >= nguess_ && val != last_value)
      break;
    else
      basis.push_back(p.second);
  }
  const int nguess = basis.size();

  shared_ptr<Matrix> spin = make_shared<CISpin>(basis, norb_);
  VectorB eigs(nguess);
  spin->diagonalize(eigs);

  int start, end;
  const double target_spin = 0.25 * static_cast<double>(det_->nspin()*(det_->nspin()+2));
  for (start = 0; start < nguess; ++start)
    if (fabs(eigs(start) - target_spin) < 1.0e-8) break;
  for (end = start; end < nguess; ++end)
    if (fabs(eigs(end) - target_spin) > 1.0e-8) break;

  if ((end-start) >= nstate_) {
    const MatView coeffs = spin->slice(start, end);

    shared_ptr<Matrix> hamiltonian = make_shared<CIHamiltonian>(basis, jop_);
    hamiltonian = make_shared<Matrix>(coeffs % *hamiltonian * coeffs);
    hamiltonian->diagonalize(eigs);

#if 0
    const double nuc_core = geom_->nuclear_repulsion() + jop_->core_energy();
    for (int i = 0; i < end-start; ++i)
      cout << setw(12) << setprecision(8) << eigs(i) + nuc_core << endl;
#endif

    auto coeffs1 = make_shared<Matrix>(coeffs * *hamiltonian);
    for (int i = 0; i < nguess; ++i) {
      const size_t ia = det_->lexical<0>(basis[i].first);
      const size_t ib = det_->lexical<1>(basis[i].second);

      for (int j = 0; j < nstate_; ++j)
        out->data(j)->element(ib, ia) = coeffs1->element(i, j);
    }
  }
  else {
    nguess_ *= 2;
    model_guess(out);
  }
}

// generate initial vectors
//   - bits: bit patterns of low-energy determinants
//   - nspin: #alpha - #beta
//   - out:
void FCI::generate_guess(const int nspin, const int nstate, shared_ptr<Dvec> out) {
  int ndet = nstate_*10;
  start_over:
  vector<pair<bitset<nbit__>, bitset<nbit__>>> bits = detseeds(ndet);

  // Spin adapt detseeds
  int oindex = 0;
  vector<bitset<nbit__>> done;
  for (auto& it : bits) {
    bitset<nbit__> alpha = it.second;
    bitset<nbit__> beta = it.first;
    bitset<nbit__> open_bit = (alpha^beta);

    // make sure that we have enough unpaired alpha
    const int unpairalpha = (alpha ^ (alpha & beta)).count();
    const int unpairbeta  = (beta ^ (alpha & beta)).count();
    if (unpairalpha-unpairbeta < nelea_-neleb_) continue;

    // check if this orbital configuration is already used
    if (find(done.begin(), done.end(), open_bit) != done.end()) continue;
    done.push_back(open_bit);

    pair<vector<tuple<int, int, int>>, double> adapt = det()->spin_adapt(nelea_-neleb_, alpha, beta);
    const double fac = adapt.second;
    for (auto& iter : adapt.first) {
      out->data(oindex)->element(get<0>(iter), get<1>(iter)) = get<2>(iter)*fac;
    }
    out->data(oindex)->spin_decontaminate();

    cout << "     guess " << setw(3) << oindex << ":   closed " <<
          setw(20) << left << print_bit(alpha&beta, norb_) << " open " << setw(20) << print_bit(open_bit, norb_) << right << endl;

    ++oindex;
    if (oindex == nstate) break;
  }
  if (oindex < nstate) {
    out->zero();
    ndet *= 4;
    goto start_over;
  }
  cout << endl;
}

// returns seed determinants for initial guess
vector<pair<bitset<nbit__> , bitset<nbit__>>> FCI::detseeds(const int ndet) {
  multimap<double, pair<bitset<nbit__>,bitset<nbit__>>> tmp;
  for (int i = 0; i != ndet; ++i) tmp.emplace(-1.0e10*(1+i), make_pair(bitset<nbit__>(0),bitset<nbit__>(0)));

  double* diter = denom_->data();
  for (auto& aiter : det()->string_bits_a()) {
    for (auto& biter : det()->string_bits_b()) {
      const double din = -(*diter);
      if (tmp.begin()->first < din) {
        tmp.emplace(din, make_pair(biter, aiter));
        tmp.erase(tmp.begin());
      }
      ++diter;
    }
  }
  assert(tmp.size() == ndet || ndet > det()->string_bits_a().size()*det()->string_bits_b().size());
  vector<pair<bitset<nbit__> , bitset<nbit__>>> out;
  for (auto iter = tmp.rbegin(); iter != tmp.rend(); ++iter)
    out.push_back(iter->second);
  return out;
}

void FCI::print_header() const {
  cout << "  ---------------------------" << endl;
  cout << "        FCI calculation      " << endl;
  cout << "  ---------------------------" << endl << endl;
}

shared_ptr<const CIWfn> FCI::conv_to_ciwfn() const {
  return make_shared<const CIWfn>(geom_, ref_->coeff(), ncore_, norb_, ref_->coeff()->mdim() - ncore_ - norb_, energy_, cc_);
}


void FCI::compute() {
  Timer pdebug(2);

  if (!restarted_) {
    // at the moment I only care about C1 symmetry, with dynamics in mind
    if (geom_->nirrep() > 1) throw runtime_error("FCI: C1 only at the moment.");

    // Creating an initial CI vector
    cc_ = make_shared<Dvec>(det_, nstate_); // B runs first

    // find determinants that have small diagonal energies
    if (nguess_ <= nstate_)
      generate_guess(nelea_-neleb_, nstate_, cc_);
    else
      model_guess(cc_);
    pdebug.tick_print("guess generation");

    // Davidson utility
    davidson_ = make_shared<DavidsonDiag<Civec>>(nstate_, davidson_subspace_);
  }

  // nuclear energy retrieved from geometry
  const double nuc_core = geom_->nuclear_repulsion() + jop_->core_energy();

  // main iteration starts here
  cout << "  === FCI iteration ===" << endl << endl;
  // 0 means not converged
  vector<int> conv(nstate_, 0);

  for (int iter = 0; iter != max_iter_; ++iter) {
    Timer fcitime;

    // form a sigma vector given cc
    shared_ptr<Dvec> sigma = form_sigma(cc_, jop_, conv);
    pdebug.tick_print("sigma vector");

#ifndef DISABLE_SERIALIZATION
    if (restart_) {
      stringstream ss; ss << "fci_" << iter;
      OArchive ar(ss.str());
      ar << static_cast<Method*>(this);
    }
#endif

    // constructing Dvec's for Davidson
    auto ccn = make_shared<const CASDvec>(cc_->dvec());
    auto sigman = make_shared<const CASDvec>(sigma->dvec());
    const vector<double> energies = davidson_->compute(ccn->dvec(conv), sigman->dvec(conv));

    // get residual and new vectors
    vector<shared_ptr<Civec>> errvec = davidson_->residual();
    pdebug.tick_print("davidson");

    // compute errors
    vector<double> errors;
    for (int i = 0; i != nstate_; ++i) {
      errors.push_back(errvec[i]->rms());
      conv[i] = static_cast<int>(errors[i] < thresh_);
    }
    pdebug.tick_print("error");

    if (!*min_element(conv.begin(), conv.end())) {
      // denominator scaling
      for (int ist = 0; ist != nstate_; ++ist) {
        if (conv[ist]) continue;
        const size_t size = cc_->data(ist)->size();
        double* target_array = cc_->data(ist)->data();
        double* source_array = errvec[ist]->data();
        double* denom_array = denom_->data();
        const double en = energies[ist];
        for (size_t i = 0; i != size; ++i) {
          target_array[i] = source_array[i] / min(en - denom_array[i], -0.1);
        }
        cc_->data(ist)->normalize();
        cc_->data(ist)->spin_decontaminate();
        cc_->data(ist)->synchronize();
      }
    }
    pdebug.tick_print("denominator");

    // printing out
    if (nstate_ != 1 && iter) cout << endl;
    for (int i = 0; i != nstate_; ++i) {
      cout << setw(7) << iter << setw(3) << i << setw(2) << (conv[i] ? "*" : " ")
                              << setw(17) << fixed << setprecision(8) << energies[i]+nuc_core << "   "
                              << setw(10) << scientific << setprecision(2) << errors[i] << fixed << setw(10) << setprecision(2)
                              << fcitime.tick() << endl;
      energy_[i] = energies[i]+nuc_core;
    }
    if (*min_element(conv.begin(), conv.end())) break;
  }
  // main iteration ends here

  auto cc = make_shared<CASDvec>(davidson_->civec());
  cc_ = make_shared<Dvec>(*cc);
  cc_->print(print_thresh_);

  for (auto& iprop : properties_) {
    iprop->compute(cc_);
    iprop->print();
  }
}
