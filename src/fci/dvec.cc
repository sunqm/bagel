//
// Newint - Parallel electron correlation program.
// Filename: dvec.cc
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

#include <src/fci/dvec.h>

using namespace std;

Dvec::Dvec(shared_ptr<const Determinants> det, const size_t ij) : det_(det), lena_(det->lena()), lenb_(det->lenb()), ij_(ij) {
  // data should be in a consecutive area to call dgemm.
  data_ = unique_ptr<double[]>(new double[lenb_*lena_*ij_]);
  double* tmp = data_.get();
  for (int i = 0; i != ij_; ++i, tmp+=lenb_*lena_) {
    shared_ptr<Civec> c(new Civec(det_, tmp)); 
    dvec_.push_back(c);
  }
}


Dvec::Dvec(const Dvec& o) : det_(o.det_), lena_(o.lena_), lenb_(o.lenb_), ij_(o.ij_) {
  data_ = unique_ptr<double[]>(new double[lena_*lenb_*ij_]);
  double* tmp = data_.get();
  for (int i = 0; i != ij_; ++i, tmp+=lenb_*lena_) {
    shared_ptr<Civec> c(new Civec(det_, tmp)); 
    dvec_.push_back(c);
  }
  std::copy(o.data(), o.data()+lena_*lenb_*ij_, data());
}


Dvec::Dvec(shared_ptr<const Civec> e, const size_t ij) : det_(e->det()), lena_(e->lena()), lenb_(e->lenb()), ij_(ij) {
  data_ = unique_ptr<double[]>(new double[lena_*lenb_*ij]);
  double* tmp = data();
  for (int i = 0; i != ij; ++i, tmp+=lenb_*lena_) {
    shared_ptr<Civec> c(new Civec(det_, tmp)); 
    dcopy_(lenb_*lena_, e->data(), 1, c->data(), 1);
    dvec_.push_back(c);
  }
}


// I think this is very confusiong... this is done this way in order not to delete Civec when Dvec is deleted. 
Dvec::Dvec(shared_ptr<const Dvec> o) : det_(o->det_), lena_(o->lena_), lenb_(o->lenb_), ij_(o->ij_) {
  for (int i = 0; i != ij_; ++i) {
    shared_ptr<Civec> c(new Civec(*(o->data(i))));
    dvec_.push_back(c);
  }
}


Dvec::Dvec(vector<shared_ptr<Civec> > o) : det_(o.front()->det()), ij_(o.size()) {
  lena_ = det_->lena();
  lenb_ = det_->lenb();
  dvec_ = o;
}


// returns a vector of Civec's which correspond to an unconverged state
vector<shared_ptr<Civec> > Dvec::dvec(const vector<int>& conv) {
  vector<shared_ptr<Civec> > out;
  int i = 0;
  for (auto iter = dvec_.begin(); iter != dvec_.end(); ++iter, ++i) {
    if (conv[i] == 0) out.push_back(*iter);
  }
  return out;
}


vector<shared_ptr<const Civec> > Dvec::dvec(const vector<int>& conv) const {
  vector<shared_ptr<const Civec> > out;
  int i = 0;
  for (auto iter = dvec_.begin(); iter != dvec_.end(); ++iter, ++i) {
    if (conv[i] == 0) out.push_back(*iter);
  }
  return out;
}


void Dvec::set_det(shared_ptr<const Determinants> o) const {
  det_ = o;
  for (auto i = dvec_.begin(); i != dvec_.end(); ++i) (*i)->set_det(o); 
}


double Dvec::ddot(const Dvec& other) const {
  assert(ij() == other.ij());
  double sum = 0.0;
  for (auto i = dvec_.begin(), j = other.dvec_.begin(); i != dvec_.end(); ++i, ++j)
    sum += ddot_(lena_*lenb_, (*i)->data(), 1, (*j)->data(), 1);
  return sum;
}


void Dvec::daxpy(double a, const Dvec& other) {
  auto j = other.dvec_.begin();
  for (auto i = dvec_.begin(); i != dvec_.end(); ++i, ++j)
    daxpy_(lena_*lenb_, a, (*j)->data(), 1, (*i)->data(), 1);
}


void Dvec::scale(const double a) {
  for (auto i = dvec_.begin(); i != dvec_.end(); ++i)
    dscal_(lena_*lenb_, a, (*i)->data(), 1);
}