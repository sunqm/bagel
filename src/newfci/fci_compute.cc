//
// BAGEL - Parallel electron correlation program.
// Filename: fci_compute.cc
// Copyright (C) 2011 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and\/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
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

//#define __NEWFCI_DEBUGGING

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <src/newfci/fci.h>
#include <src/newfci/space.h>
#include <src/util/davidson.h>
#include <src/util/constants.h>
#include <vector>

// toggle for timing print out.
static const bool tprint = true;

using namespace std;
using namespace std::chrono;
using namespace bagel;

static shared_ptr<Space> space;

void NewFCI::update(shared_ptr<const Coeff> c) {

  // iiii file to be created (MO transformation).
  // now jop_->mo1e() and jop_->mo2e() contains one and two body part of Hamiltonian
  auto tp1 = high_resolution_clock::now();
  jop_ = shared_ptr<NewMOFile>(new NewJop(ref_, ncore_, ncore_+norb_, c));

  // right now full basis is used. 
  auto tp2 = high_resolution_clock::now();
  cout << "    * Integral transformation done. Elapsed time: " << setprecision(2) <<
          duration_cast<milliseconds>(tp2-tp1).count()*0.001 << endl << endl;

  // create denominator here. Stored in shared<NewCivec> denom_
  const_denom();

}

void NewFCI::compute() {

  // at the moment I only care about C1 symmetry, with dynamics in mind
  if (geom_->nirrep() > 1) throw runtime_error("NewFCI: C1 only at the moment."); 

  // some constants
  const int ij = nij(); 

  // Creating an initial CI vector
  shared_ptr<NewDvec> cc_tmp(new NewDvec(det_, nstate_)); // B runs first
  cc_ = cc_tmp;

  // find determinants that have small diagonal energies
  generate_guess(nelea_-neleb_, nstate_, cc_); 
  // TODO note that generate_guess is only working fine for singlets

  // nuclear energy retrieved from geometry
  const double nuc_core = geom_->nuclear_repulsion() + jop_->core_energy();

  // construct space
  space = shared_ptr<Space>(new Space(det_,1));

  // Davidson utility
  DavidsonDiag<NewCivec> davidson(nstate_, max_iter_);

  // main iteration starts here
  cout << "  === NewFCI iteration ===" << endl << endl;
  // 0 means not converged
  vector<int> conv(nstate_,0);

  for (int iter = 0; iter != max_iter_; ++iter) { 
    auto tp1 = high_resolution_clock::now();

    // form a sigma vector given cc
    cout << "probably see this" << endl;
    shared_ptr<NewDvec> sigma = form_sigma(cc_, jop_, conv);
    cout << "and not this" << endl;

    // constructing NewDvec's for Davidson
    shared_ptr<const NewDvec> ccn(new NewDvec(cc_));
    shared_ptr<const NewDvec> sigman(new NewDvec(sigma));
    const vector<double> energies = davidson.compute(ccn->dvec(conv), sigman->dvec(conv));

    // get residual and new vectors
    vector<shared_ptr<NewCivec> > errvec = davidson.residual();

    // compute errors
    vector<double> errors;
    for (int i = 0; i != nstate_; ++i) {
      errors.push_back(errvec[i]->variance());
      conv[i] = static_cast<int>(errors[i] < thresh_);
    }

    if (!*min_element(conv.begin(), conv.end())) {
      // denominator scaling 
      for (int ist = 0; ist != nstate_; ++ist) {
        if (conv[ist]) continue;
        const int size = cc_->data(ist)->size();
        double* target_array = cc_->data(ist)->data();
        double* source_array = errvec[ist]->data();
        double* denom_array = denom_->data();
        const double en = energies[ist];
        for (int i = 0; i != size; ++i) {
          target_array[i] = source_array[i] / min(en - denom_array[i], -0.1);
        }
        davidson.orthog(cc_->data(ist));
        list<shared_ptr<const NewCivec> > tmp;
        for (int jst = 0; jst != ist; ++jst) tmp.push_back(cc_->data(jst)); 
        cc_->data(ist)->orthog(tmp);
      }
    }

    // printing out
    auto tp2 = high_resolution_clock::now();
    auto dr = duration_cast<milliseconds>(tp2-tp1);
    if (nstate_ != 1 && iter) cout << endl;
    for (int i = 0; i != nstate_; ++i) {
      cout << setw(7) << iter << setw(3) << i << setw(2) << (conv[i] ? "*" : " ")
                              << setw(17) << fixed << setprecision(8) << energies[i]+nuc_core << "   "
                              << setw(10) << scientific << setprecision(2) << errors[i] << fixed << setw(10) << setprecision(2)
                              << dr.count()*0.001 << endl; 
      energy_[i] = energies[i]+nuc_core;
    }
    if (*min_element(conv.begin(), conv.end())) break;
  }
  // main iteration ends here

  shared_ptr<NewDvec> s(new NewDvec(davidson.civec()));
  s->print();
  cc_ = shared_ptr<NewDvec>(new NewDvec(s));

}




#ifndef __NEWFCI_DEBUGGING
shared_ptr<NewDvec> NewFCI::form_sigma(shared_ptr<const NewDvec> ccvec, shared_ptr<const NewMOFile> jop,
                     const vector<int>& conv) const { // d and e are scratch area for D and E intermediates 

  const int ij = nij(); 

  const int nstate = ccvec->ij();
  shared_ptr<NewDvec> sigmavec(new NewDvec(ccvec->det(), nstate));
  sigmavec->zero();

  // we need two vectors for intermediate quantities
  shared_ptr<NewDvec> d(new NewDvec(ccvec->det(), ij));
  shared_ptr<NewDvec> e(new NewDvec(ccvec->det(), ij));


  for (int istate = 0; istate != nstate; ++istate) {
    if (conv[istate]) continue;
    shared_ptr<const NewCivec> cc = ccvec->data(istate);  
    shared_ptr<NewCivec> sigma = sigmavec->data(istate);  

    vector<pair<string, double> > timing;
    int start = ::clock();

    // (task1) one-electron alpha: sigma(Psib, Psi'a) += sign h'(ij) C(Psib, Psia) 
    sigma_1(cc, sigma, jop);
    if (tprint) print_timing_("task1", start, timing);

    // (task2) two electron contributions
    d->zero();

    // step (c) (task2a-1) D(Phib, Phia, ij) += sign C(Psib, Phi'a)
    sigma_2a1(cc, d);
    if (tprint) print_timing_("task2a-1", start, timing);

    // step (d) (task2a-2) D(Phib, Phia, ij) += sign C(Psib', Phia)
    sigma_2a2(cc, d);
    if (tprint) print_timing_("task2a-2", start, timing);

    // step (e) (task2b) E(Phib, Phia, kl) = D(Psib, Phia, ij) (ij|kl)
    sigma_2b(d, e, jop);
    if (tprint) print_timing_("task2b", start, timing);

    // step (f) (task2c-1) sigma(Phib, Phia') += sign E(Psib, Phia, kl)
    sigma_2c1(sigma, e);
    if (tprint) print_timing_("task2c-1", start, timing);

    // step (g) (task2c-2) sigma(Phib', Phia) += sign E(Psib, Phia, kl)
    sigma_2c2(sigma, e);
    if (tprint) print_timing_("task2c-2", start, timing);

    // (task3) one-electron beta: sigma(Psib', Psia) += sign h'(ij) C(Psib, Psia)
    sigma_3(cc, sigma, jop);

    if (tprint) {
      print_timing_("task3", start, timing);
      cout << "     timing info" << endl;
      for (auto iter = timing.begin(); iter != timing.end(); ++iter)
        cout << "    " << setw(10) << iter->first << setw(10) << setprecision(2) << iter ->second << endl;
    }
  }

  return sigmavec;
}
#else
/* This part is only intended for debugging the Space functions. It should probably be reworked if performance is desired. */
/* Implementing the method as described by Harrison and Zarrabian */
shared_ptr<NewDvec> NewFCI::form_sigma(shared_ptr<const NewDvec> ccvec, shared_ptr<const NewMOFile> jop,
                     const vector<int>& conv) const { // d and e are scratch area for D and E intermediates 

  const int norb = norb_;
  const int ij = norb*norb; 

  const int nstate = ccvec->ij();
  shared_ptr<NewDvec> sigmavec(new NewDvec(ccvec->det(), nstate));
  sigmavec->zero();

  //static shared_ptr<Space> space(new Space(ccvec->det(), 1));

  shared_ptr<NewDeterminants> base_det = space->finddet(0,0);
  shared_ptr<NewDeterminants> int_det = space->finddet(-1,-1);

  /* d and e are only used in the alpha-beta case and exist in the (nalpha-1)(nbeta-1) spaces */
  shared_ptr<NewDvec> d(new NewDvec(int_det, ij));
  shared_ptr<NewDvec> e(new NewDvec(int_det, ij));

  for (int istate = 0; istate != nstate; ++istate) {
    if (conv[istate]) continue;
    shared_ptr<const NewCivec> cc = ccvec->data(istate);  
    shared_ptr<NewCivec> sigma = sigmavec->data(istate);  

    vector<pair<string, double> > timing;
    int start = ::clock();

    // (task1) one-electron alpha: sigma(Psib, Psi'a) += sign h'(ij) C(Psib, Psia) 
    sigma_1(cc, sigma, jop);
    if (tprint) print_timing_("task1", start, timing);

    // (task2) two electron contributions

    // (2aa) alpha-alpha contributions
    /* Apparently the advantage to having this loop structure is the top loop can be threaded easily */
    {
      const double* const source_base = cc->data();
      double* target_base = sigma->data();
      int lb = sigma->lenb();
      
      for (auto aiter = int_det->stringa().begin(); aiter != int_det->stringa().end(); ++aiter) {
        bitset<nbit__> nstring = *aiter;
        for (int i = 0; i != norb; ++i) {
          if (!nstring[i]) continue;
          for (int j = 0; j < i; ++j) {
            if(!nstring[j]) continue;
            int ij_phase = int_det->sign(nstring,i,j);
            bitset<nbit__> string_ij = nstring; 
            string_ij.reset(i); string_ij.reset(j);
            for (int k = 0; k != norb; ++k) {
              if (string_ij[k]) continue;
              for (int l = 0; l < k; ++l) {
                if (string_ij[l]) continue;
                int kl_phase = int_det->sign(string_ij,k,l);
                double phase = static_cast<double>(ij_phase*kl_phase);
                bitset<nbit__> string_ijkl = string_ij;
                string_ijkl.set(k); string_ijkl.set(l);
                double temp = phase * ( jop->mo2e(i,j,k,l) - jop->mo2e(i,l,k,j) );
                const double* source = source_base + base_det->lexical<0>(string_ijkl)*lb;
                daxpy_(lb, temp, source, 1, target_base, 1);
              }
            }
          }
        }
        target_base += lb;
      }
    }
    if (tprint) print_timing_("task2aa", start, timing);
    
    // (2bb) beta-beta contributions
    /* Mostly the same as the alpha-alpha, except for data storage */
    {
      const double* const source_base = cc->data();
      double* target_base = sigma->data();
      int la = sigma->lena();
      int lb = sigma->lenb();
      for (auto biter = cc->det()->stringb().begin(); biter != cc->det()->stringb().end(); ++biter,++target_base) {
        bitset<nbit__> nstring = *biter;
        for (int i = 0; i != norb; ++i) {
          if (!nstring[i]) continue;
          for (int j = 0; j < i; ++j) {
            if(!nstring[j]) continue;
            int ij_phase = int_det->sign(nstring,i,j);
            bitset<nbit__> string_ij = nstring;
            string_ij.reset(i); string_ij.reset(j);
            for (int k = 0; k != norb; ++k) {
              if (string_ij[k]) continue;
              for (int l = 0; l < k; ++l) {
                if (string_ij[l]) continue;
                int kl_phase = int_det->sign(string_ij,k,l);
                double phase = static_cast<double>(ij_phase*kl_phase);
                bitset<nbit__> string_ijkl = string_ij;
                string_ijkl.set(k); string_ijkl.set(l);
                double temp = phase * ( jop->mo2e(i,j,k,l) - jop->mo2e(i,l,k,j) );
                const double* source = source_base + base_det->lexical<1>(string_ijkl);
                daxpy_(la, temp, source, lb, target_base, lb);
              }
            }
          }
        }
      }
    }
    if (tprint) print_timing_("task2bb", start, timing);

    // (2ab) alpha-beta contributions
    /* Resembles more the Knowles & Handy FCI terms */
    d->zero();
    e->zero();
    {
      const int lbt = d->lenb();
      const int lbs = cc->lenb();
      const double* source_base = cc->data();

      for (int i = 0; i < norb; ++i) {
        for (int j = 0; j < norb; ++j) {
          double* target_base = d->data(i*norb + j)->data();
          for (auto aiter = d->det()->phiupa(i).begin(); aiter != d->det()->phiupa(i).end(); ++aiter) {
            for (auto biter = d->det()->phiupb(j).begin(); biter != d->det()->phiupb(j).end(); ++biter) {
              const double sign = static_cast<double>(get<1>(*aiter)*get<1>(*biter));
              target_base[get<2>(*aiter)*lbt + get<2>(*biter)] += sign * source_base[get<0>(*aiter)*lbs + get<0>(*biter)];
            }
          }
        }
      }
    }
    if (tprint) print_timing_("task2ab-1", start, timing);
  cout << "please" << endl;

    {
      const int lenab = int_det->lena() * int_det->lenb();
      const int ij = d->ij();
      // TODO: check 0.5 coefficient.
      dgemm_("n", "n", lenab, ij, ij, 0.5, d->data(), lenab, jop->mo2e_ptr(), ij, 0.0, e->data(), lenab);
    }
    if (tprint) print_timing_("task2ab-2", start, timing);

    {
      const int lbt = base_det->lenb();
      const int lbs = int_det->lenb();
      double* target_base = sigma->data();

      for (int k = 0; k < norb; ++k) {
        for (int l = 0; l < norb; ++l) {
          const double* source_base = e->data(k*norb + l)->data();
          for (auto aiter = int_det->phiupa(k).begin(); aiter != int_det->phiupa(k).end(); ++aiter) {
            for (auto biter = int_det->phiupb(l).begin(); biter != int_det->phiupb(l).end(); ++biter) {
              const double sign = static_cast<double>(get<1>(*aiter)*get<1>(*biter));
              target_base[get<0>(*aiter)*lbt + get<0>(*biter)] += sign * source_base[get<2>(*aiter)*lbs + get<2>(*biter)];
            }
          } 
        }
      }
    }
    if (tprint) print_timing_("task2ab-3", start, timing);
    
    // (task3) one-electron beta: sigma(Psib', Psia) += sign h'(ij) C(Psib, Psia)
    sigma_3(cc, sigma, jop);

    if (tprint) {
      print_timing_("task3", start, timing);
      cout << "     timing info" << endl;
      for (auto iter = timing.begin(); iter != timing.end(); ++iter)
        cout << "    " << setw(10) << iter->first << setw(10) << setprecision(2) << iter ->second << endl;
    }
  cout << "there" << endl;
  }

  return sigmavec;
}
#endif


void NewFCI::sigma_1(shared_ptr<const NewCivec> cc, shared_ptr<NewCivec> sigma, shared_ptr<const NewMOFile> jop) const {
  assert(cc->det() == sigma->det());
  const int ij = nij(); 
  const int lb = cc->lenb();
  for (int ip = 0; ip != ij; ++ip) {
    const double h = jop->mo1e(ip);
    for (auto iter = cc->det()->phia(ip).begin();  iter != cc->det()->phia(ip).end(); ++iter) {
      const double hc = h * get<1>(*iter);
      daxpy_(lb, hc, cc->element_ptr(0, get<2>(*iter)), 1, sigma->element_ptr(0, get<0>(*iter)), 1); 
    }
  }
}

void NewFCI::sigma_2a1(shared_ptr<const NewCivec> cc, shared_ptr<NewDvec> d) const {
  assert(d->det() == cc->det());
  const int lb = d->lenb();
  const int ij = d->ij();
  const double* const source_base = cc->data();
  for (int ip = 0; ip != ij; ++ip) {
    double* const target_base = d->data(ip)->data();
    for (auto iter = cc->det()->phia(ip).begin();  iter != cc->det()->phia(ip).end(); ++iter) {
      const double sign = static_cast<double>(get<1>(*iter));
      double* const target_array = target_base + get<2>(*iter)*lb;
      daxpy_(lb, sign, source_base + get<0>(*iter)*lb, 1, target_array, 1);
    }
  }
}

void NewFCI::sigma_2a2(shared_ptr<const NewCivec> cc, shared_ptr<NewDvec> d) const {
  assert(d->det() == cc->det());
  const int la = d->lena();
  const int ij = d->ij();
  for (int i = 0; i < la; ++i) {
    const double* const source_array0 = cc->element_ptr(0, i);
    for (int ip = 0; ip != ij; ++ip) {
      double* const target_array0 = d->data(ip)->element_ptr(0, i);
      for (auto iter = cc->det()->phib(ip).begin(); iter != cc->det()->phib(ip).end(); ++iter) {
        const double sign = static_cast<double>(get<1>(*iter));
        target_array0[get<2>(*iter)] += sign * source_array0[get<0>(*iter)]; 
      }
    }
  }
}

void NewFCI::sigma_2c1(shared_ptr<NewCivec> sigma, shared_ptr<const NewDvec> e) const {
  const int lb = e->lenb();
  const int ij = e->ij();
  for (int ip = 0; ip != ij; ++ip) { 
    const double* const source_base = e->data(ip)->data();
    for (auto iter = e->det()->phia(ip).begin(); iter != e->det()->phia(ip).end(); ++iter) {
      const double sign = static_cast<double>(get<1>(*iter)); 
      double* const target_array = sigma->element_ptr(0, get<0>(*iter));
      daxpy_(lb, sign, source_base + lb*get<2>(*iter), 1, target_array, 1);
    }
  }
}

void NewFCI::sigma_2c2(shared_ptr<NewCivec> sigma, shared_ptr<const NewDvec> e) const {
  const int la = e->lena();
  const int ij = e->ij();
  for (int i = 0; i < la; ++i) {
    double* const target_array0 = sigma->element_ptr(0, i);
    for (int ip = 0; ip != ij; ++ip) {
      const double* const source_array0 = e->data(ip)->element_ptr(0, i);
      for (auto iter = e->det()->phib(ip).begin(); iter != e->det()->phib(ip).end(); ++iter) {
        const double sign = static_cast<double>(get<1>(*iter));
        target_array0[get<0>(*iter)] += sign * source_array0[get<2>(*iter)]; 
      }
    }
  }
}


void NewFCI::sigma_3(shared_ptr<const NewCivec> cc, shared_ptr<NewCivec> sigma, shared_ptr<const NewMOFile> jop) const {
  const int la = cc->lena();
  const int ij = nij();

  for (int i = 0; i < la; ++i) {
    double* const target_array0 = sigma->element_ptr(0, i);
    const double* const source_array0 = cc->element_ptr(0, i);
    for (int ip = 0; ip != ij; ++ip) {
      const double h = jop->mo1e(ip);
      for (auto iter = cc->det()->phib(ip).begin();  iter != cc->det()->phib(ip).end(); ++iter) {
        const double hc = h * get<1>(*iter);
        target_array0[get<0>(*iter)] += hc * source_array0[get<2>(*iter)];
      }
    }
  }
}


void NewFCI::sigma_2b(shared_ptr<NewDvec> d, shared_ptr<NewDvec> e, shared_ptr<const NewMOFile> jop) const {
  const int la = d->lena();
  const int lb = d->lenb();
  const int ij = d->ij();
  const int lenab = la*lb;
  dgemm_("n", "n", lenab, ij, ij, 0.5, d->data(), lenab, jop->mo2e_ptr(), ij,
                                  0.0, e->data(), lenab);
}
