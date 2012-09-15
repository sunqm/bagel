//
// BAGEL - Parallel electron correlation program.
// Filename: _svrr_60a0.cc
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

// returns double array of length 693
void SVRRList::_svrr_60a0(double* data_, const double* C00_, const double* D00_, const double* B00_, const double* B01_, const double* B10_) {
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
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[36+t] = C00_[t] * data_[27+t] + B10_current[t] * data_[18+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[45+t] = C00_[t] * data_[36+t] + B10_current[t] * data_[27+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[54+t] = C00_[t] * data_[45+t] + B10_current[t] * data_[36+t];

  for (int t = 0; t != 9; ++t)
    data_[63+t] = D00_[t];

  double cB00_current[9];
  for (int t = 0; t != 9; ++t)
    cB00_current[t] = B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[72+t] = C00_[t] * data_[63+t] + cB00_current[t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[81+t] = C00_[t] * data_[72+t] + B10_current[t] * data_[63+t] + cB00_current[t] * data_[9+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[90+t] = C00_[t] * data_[81+t] + B10_current[t] * data_[72+t] + cB00_current[t] * data_[18+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[99+t] = C00_[t] * data_[90+t] + B10_current[t] * data_[81+t] + cB00_current[t] * data_[27+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[108+t] = C00_[t] * data_[99+t] + B10_current[t] * data_[90+t] + cB00_current[t] * data_[36+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[117+t] = C00_[t] * data_[108+t] + B10_current[t] * data_[99+t] + cB00_current[t] * data_[45+t];

  double B01_current[9];
  for (int t = 0; t != 9; ++t)
    B01_current[t] = B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[126+t] = D00_[t] * data_[63+t] + B01_current[t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[135+t] = C00_[t] * data_[126+t] + cB00_current[t] * data_[63+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[144+t] = C00_[t] * data_[135+t] + B10_current[t] * data_[126+t] + cB00_current[t] * data_[72+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[153+t] = C00_[t] * data_[144+t] + B10_current[t] * data_[135+t] + cB00_current[t] * data_[81+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[162+t] = C00_[t] * data_[153+t] + B10_current[t] * data_[144+t] + cB00_current[t] * data_[90+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[171+t] = C00_[t] * data_[162+t] + B10_current[t] * data_[153+t] + cB00_current[t] * data_[99+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[180+t] = C00_[t] * data_[171+t] + B10_current[t] * data_[162+t] + cB00_current[t] * data_[108+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[189+t] = D00_[t] * data_[126+t] + B01_current[t] * data_[63+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[198+t] = C00_[t] * data_[189+t] + cB00_current[t] * data_[126+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[207+t] = C00_[t] * data_[198+t] + B10_current[t] * data_[189+t] + cB00_current[t] * data_[135+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[216+t] = C00_[t] * data_[207+t] + B10_current[t] * data_[198+t] + cB00_current[t] * data_[144+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[225+t] = C00_[t] * data_[216+t] + B10_current[t] * data_[207+t] + cB00_current[t] * data_[153+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[234+t] = C00_[t] * data_[225+t] + B10_current[t] * data_[216+t] + cB00_current[t] * data_[162+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[243+t] = C00_[t] * data_[234+t] + B10_current[t] * data_[225+t] + cB00_current[t] * data_[171+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[252+t] = D00_[t] * data_[189+t] + B01_current[t] * data_[126+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[261+t] = C00_[t] * data_[252+t] + cB00_current[t] * data_[189+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[270+t] = C00_[t] * data_[261+t] + B10_current[t] * data_[252+t] + cB00_current[t] * data_[198+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[279+t] = C00_[t] * data_[270+t] + B10_current[t] * data_[261+t] + cB00_current[t] * data_[207+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[288+t] = C00_[t] * data_[279+t] + B10_current[t] * data_[270+t] + cB00_current[t] * data_[216+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[297+t] = C00_[t] * data_[288+t] + B10_current[t] * data_[279+t] + cB00_current[t] * data_[225+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[306+t] = C00_[t] * data_[297+t] + B10_current[t] * data_[288+t] + cB00_current[t] * data_[234+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[315+t] = D00_[t] * data_[252+t] + B01_current[t] * data_[189+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[324+t] = C00_[t] * data_[315+t] + cB00_current[t] * data_[252+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[333+t] = C00_[t] * data_[324+t] + B10_current[t] * data_[315+t] + cB00_current[t] * data_[261+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[342+t] = C00_[t] * data_[333+t] + B10_current[t] * data_[324+t] + cB00_current[t] * data_[270+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[351+t] = C00_[t] * data_[342+t] + B10_current[t] * data_[333+t] + cB00_current[t] * data_[279+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[360+t] = C00_[t] * data_[351+t] + B10_current[t] * data_[342+t] + cB00_current[t] * data_[288+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[369+t] = C00_[t] * data_[360+t] + B10_current[t] * data_[351+t] + cB00_current[t] * data_[297+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[378+t] = D00_[t] * data_[315+t] + B01_current[t] * data_[252+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[387+t] = C00_[t] * data_[378+t] + cB00_current[t] * data_[315+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[396+t] = C00_[t] * data_[387+t] + B10_current[t] * data_[378+t] + cB00_current[t] * data_[324+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[405+t] = C00_[t] * data_[396+t] + B10_current[t] * data_[387+t] + cB00_current[t] * data_[333+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[414+t] = C00_[t] * data_[405+t] + B10_current[t] * data_[396+t] + cB00_current[t] * data_[342+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[423+t] = C00_[t] * data_[414+t] + B10_current[t] * data_[405+t] + cB00_current[t] * data_[351+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[432+t] = C00_[t] * data_[423+t] + B10_current[t] * data_[414+t] + cB00_current[t] * data_[360+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[441+t] = D00_[t] * data_[378+t] + B01_current[t] * data_[315+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[450+t] = C00_[t] * data_[441+t] + cB00_current[t] * data_[378+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[459+t] = C00_[t] * data_[450+t] + B10_current[t] * data_[441+t] + cB00_current[t] * data_[387+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[468+t] = C00_[t] * data_[459+t] + B10_current[t] * data_[450+t] + cB00_current[t] * data_[396+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[477+t] = C00_[t] * data_[468+t] + B10_current[t] * data_[459+t] + cB00_current[t] * data_[405+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[486+t] = C00_[t] * data_[477+t] + B10_current[t] * data_[468+t] + cB00_current[t] * data_[414+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[495+t] = C00_[t] * data_[486+t] + B10_current[t] * data_[477+t] + cB00_current[t] * data_[423+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[504+t] = D00_[t] * data_[441+t] + B01_current[t] * data_[378+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[513+t] = C00_[t] * data_[504+t] + cB00_current[t] * data_[441+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[522+t] = C00_[t] * data_[513+t] + B10_current[t] * data_[504+t] + cB00_current[t] * data_[450+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[531+t] = C00_[t] * data_[522+t] + B10_current[t] * data_[513+t] + cB00_current[t] * data_[459+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[540+t] = C00_[t] * data_[531+t] + B10_current[t] * data_[522+t] + cB00_current[t] * data_[468+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[549+t] = C00_[t] * data_[540+t] + B10_current[t] * data_[531+t] + cB00_current[t] * data_[477+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[558+t] = C00_[t] * data_[549+t] + B10_current[t] * data_[540+t] + cB00_current[t] * data_[486+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[567+t] = D00_[t] * data_[504+t] + B01_current[t] * data_[441+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[576+t] = C00_[t] * data_[567+t] + cB00_current[t] * data_[504+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[585+t] = C00_[t] * data_[576+t] + B10_current[t] * data_[567+t] + cB00_current[t] * data_[513+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[594+t] = C00_[t] * data_[585+t] + B10_current[t] * data_[576+t] + cB00_current[t] * data_[522+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[603+t] = C00_[t] * data_[594+t] + B10_current[t] * data_[585+t] + cB00_current[t] * data_[531+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[612+t] = C00_[t] * data_[603+t] + B10_current[t] * data_[594+t] + cB00_current[t] * data_[540+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[621+t] = C00_[t] * data_[612+t] + B10_current[t] * data_[603+t] + cB00_current[t] * data_[549+t];

  for (int t = 0; t != 9; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 9; ++t)
    data_[630+t] = D00_[t] * data_[567+t] + B01_current[t] * data_[504+t];

  for (int t = 0; t != 9; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 9; ++t)
    data_[639+t] = C00_[t] * data_[630+t] + cB00_current[t] * data_[567+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] = B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[648+t] = C00_[t] * data_[639+t] + B10_current[t] * data_[630+t] + cB00_current[t] * data_[576+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[657+t] = C00_[t] * data_[648+t] + B10_current[t] * data_[639+t] + cB00_current[t] * data_[585+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[666+t] = C00_[t] * data_[657+t] + B10_current[t] * data_[648+t] + cB00_current[t] * data_[594+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[675+t] = C00_[t] * data_[666+t] + B10_current[t] * data_[657+t] + cB00_current[t] * data_[603+t];

  for (int t = 0; t != 9; ++t)
    B10_current[t] += B10_[t];

  for (int t = 0; t != 9; ++t)
    data_[684+t] = C00_[t] * data_[675+t] + B10_current[t] * data_[666+t] + cB00_current[t] * data_[612+t];
}

