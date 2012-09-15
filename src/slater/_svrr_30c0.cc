//
// BAGEL - Parallel electron correlation program.
// Filename: _svrr_30c0.cc
// Copyright (C) 2009 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and/or modify
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

#include "svrrlist.h"

using namespace bagel;

// returns double array of length 468
void SVRRList::_svrr_30c0(double* data_, const double* C00_, const double* D00_, const double* B00_, const double* B01_, const double* B10_) {
  for (int t = 0; t != 9; ++t)
    data_[0+t] = 1.0;

  for (int t = 0; t != 9; ++t)
    data_[9+t] = C00_[t];

  double B10_current[9];
  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[18+t] = C00_[t] * data_[9+t] + B10_current[t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[27+t] = C00_[t] * data_[18+t] + B10_current[t] * data_[9+t];

  for (int t = 0; t != 9; ++t)
    data_[36+t] = D00_[t];

  double cB00_current[9];
  for (int t = 0; t != 9; ++t)
    cB00_current[t] = B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[45+t] = C00_[t] * data_[36+t] + cB00_current[t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[54+t] = C00_[t] * data_[45+t] + B10_current[t] * data_[36+t] + cB00_current[t] * data_[9+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[63+t] = C00_[t] * data_[54+t] + B10_current[t] * data_[45+t] + cB00_current[t] * data_[18+t];

  double B01_current[9];
  for (int t = 0; t != 9; ++t)
    B01_current[t] = B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[72+t] = D00_[t] * data_[36+t] + B01_current[t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[81+t] = C00_[t] * data_[72+t] + cB00_current[t] * data_[36+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[90+t] = C00_[t] * data_[81+t] + B10_current[t] * data_[72+t] + cB00_current[t] * data_[45+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[99+t] = C00_[t] * data_[90+t] + B10_current[t] * data_[81+t] + cB00_current[t] * data_[54+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[108+t] = D00_[t] * data_[72+t] + B01_current[t] * data_[36+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[117+t] = C00_[t] * data_[108+t] + cB00_current[t] * data_[72+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[126+t] = C00_[t] * data_[117+t] + B10_current[t] * data_[108+t] + cB00_current[t] * data_[81+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[135+t] = C00_[t] * data_[126+t] + B10_current[t] * data_[117+t] + cB00_current[t] * data_[90+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[144+t] = D00_[t] * data_[108+t] + B01_current[t] * data_[72+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[153+t] = C00_[t] * data_[144+t] + cB00_current[t] * data_[108+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[162+t] = C00_[t] * data_[153+t] + B10_current[t] * data_[144+t] + cB00_current[t] * data_[117+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[171+t] = C00_[t] * data_[162+t] + B10_current[t] * data_[153+t] + cB00_current[t] * data_[126+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[180+t] = D00_[t] * data_[144+t] + B01_current[t] * data_[108+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[189+t] = C00_[t] * data_[180+t] + cB00_current[t] * data_[144+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[198+t] = C00_[t] * data_[189+t] + B10_current[t] * data_[180+t] + cB00_current[t] * data_[153+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[207+t] = C00_[t] * data_[198+t] + B10_current[t] * data_[189+t] + cB00_current[t] * data_[162+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[216+t] = D00_[t] * data_[180+t] + B01_current[t] * data_[144+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[225+t] = C00_[t] * data_[216+t] + cB00_current[t] * data_[180+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[234+t] = C00_[t] * data_[225+t] + B10_current[t] * data_[216+t] + cB00_current[t] * data_[189+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[243+t] = C00_[t] * data_[234+t] + B10_current[t] * data_[225+t] + cB00_current[t] * data_[198+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[252+t] = D00_[t] * data_[216+t] + B01_current[t] * data_[180+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[261+t] = C00_[t] * data_[252+t] + cB00_current[t] * data_[216+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[270+t] = C00_[t] * data_[261+t] + B10_current[t] * data_[252+t] + cB00_current[t] * data_[225+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[279+t] = C00_[t] * data_[270+t] + B10_current[t] * data_[261+t] + cB00_current[t] * data_[234+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[288+t] = D00_[t] * data_[252+t] + B01_current[t] * data_[216+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[297+t] = C00_[t] * data_[288+t] + cB00_current[t] * data_[252+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[306+t] = C00_[t] * data_[297+t] + B10_current[t] * data_[288+t] + cB00_current[t] * data_[261+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[315+t] = C00_[t] * data_[306+t] + B10_current[t] * data_[297+t] + cB00_current[t] * data_[270+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[324+t] = D00_[t] * data_[288+t] + B01_current[t] * data_[252+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[333+t] = C00_[t] * data_[324+t] + cB00_current[t] * data_[288+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[342+t] = C00_[t] * data_[333+t] + B10_current[t] * data_[324+t] + cB00_current[t] * data_[297+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[351+t] = C00_[t] * data_[342+t] + B10_current[t] * data_[333+t] + cB00_current[t] * data_[306+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[360+t] = D00_[t] * data_[324+t] + B01_current[t] * data_[288+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[369+t] = C00_[t] * data_[360+t] + cB00_current[t] * data_[324+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[378+t] = C00_[t] * data_[369+t] + B10_current[t] * data_[360+t] + cB00_current[t] * data_[333+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[387+t] = C00_[t] * data_[378+t] + B10_current[t] * data_[369+t] + cB00_current[t] * data_[342+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[396+t] = D00_[t] * data_[360+t] + B01_current[t] * data_[324+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[405+t] = C00_[t] * data_[396+t] + cB00_current[t] * data_[360+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[414+t] = C00_[t] * data_[405+t] + B10_current[t] * data_[396+t] + cB00_current[t] * data_[369+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[423+t] = C00_[t] * data_[414+t] + B10_current[t] * data_[405+t] + cB00_current[t] * data_[378+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[432+t] = D00_[t] * data_[396+t] + B01_current[t] * data_[360+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[441+t] = C00_[t] * data_[432+t] + cB00_current[t] * data_[396+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[450+t] = C00_[t] * data_[441+t] + B10_current[t] * data_[432+t] + cB00_current[t] * data_[405+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[459+t] = C00_[t] * data_[450+t] + B10_current[t] * data_[441+t] + cB00_current[t] * data_[414+t];
}

