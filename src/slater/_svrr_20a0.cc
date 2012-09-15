//
// BAGEL - Parallel electron correlation program.
// Filename: _svrr_20a0.cc
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

// returns double array of length 231
void SVRRList::_svrr_20a0(double* data_, const double* C00_, const double* D00_, const double* B00_, const double* B01_, const double* B10_) {
  for (int t = 0; t != 7; ++t)
    data_[0+t] = 1.0;

  for (int t = 0; t != 7; ++t)
    data_[7+t] = C00_[t];

  for (int t = 0; t != 7; ++t)
    data_[14+t] = C00_[t] * data_[7+t] + B10_[t];

  for (int t = 0; t != 7; ++t)
    data_[21+t] = D00_[t];

  double cB00_current[7];
  for (int t = 0; t != 7; ++t)
    cB00_current[t] = B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[28+t] = C00_[t] * data_[21+t] + cB00_current[t];

  for (int t = 0; t != 7; ++t)
    data_[35+t] = C00_[t] * data_[28+t] + B10_[t] * data_[21+t] + cB00_current[t] * data_[7+t];

  double B01_current[7];
  for (int t = 0; t != 7; ++t)
    B01_current[t] = B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[42+t] = D00_[t] * data_[21+t] + B01_current[t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[49+t] = C00_[t] * data_[42+t] + cB00_current[t] * data_[21+t];

  for (int t = 0; t != 7; ++t)
    data_[56+t] = C00_[t] * data_[49+t] + B10_[t] * data_[42+t] + cB00_current[t] * data_[28+t];

  for (int t = 0; t != 7; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[63+t] = D00_[t] * data_[42+t] + B01_current[t] * data_[21+t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[70+t] = C00_[t] * data_[63+t] + cB00_current[t] * data_[42+t];

  for (int t = 0; t != 7; ++t)
    data_[77+t] = C00_[t] * data_[70+t] + B10_[t] * data_[63+t] + cB00_current[t] * data_[49+t];

  for (int t = 0; t != 7; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[84+t] = D00_[t] * data_[63+t] + B01_current[t] * data_[42+t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[91+t] = C00_[t] * data_[84+t] + cB00_current[t] * data_[63+t];

  for (int t = 0; t != 7; ++t)
    data_[98+t] = C00_[t] * data_[91+t] + B10_[t] * data_[84+t] + cB00_current[t] * data_[70+t];

  for (int t = 0; t != 7; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[105+t] = D00_[t] * data_[84+t] + B01_current[t] * data_[63+t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[112+t] = C00_[t] * data_[105+t] + cB00_current[t] * data_[84+t];

  for (int t = 0; t != 7; ++t)
    data_[119+t] = C00_[t] * data_[112+t] + B10_[t] * data_[105+t] + cB00_current[t] * data_[91+t];

  for (int t = 0; t != 7; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[126+t] = D00_[t] * data_[105+t] + B01_current[t] * data_[84+t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[133+t] = C00_[t] * data_[126+t] + cB00_current[t] * data_[105+t];

  for (int t = 0; t != 7; ++t)
    data_[140+t] = C00_[t] * data_[133+t] + B10_[t] * data_[126+t] + cB00_current[t] * data_[112+t];

  for (int t = 0; t != 7; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[147+t] = D00_[t] * data_[126+t] + B01_current[t] * data_[105+t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[154+t] = C00_[t] * data_[147+t] + cB00_current[t] * data_[126+t];

  for (int t = 0; t != 7; ++t)
    data_[161+t] = C00_[t] * data_[154+t] + B10_[t] * data_[147+t] + cB00_current[t] * data_[133+t];

  for (int t = 0; t != 7; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[168+t] = D00_[t] * data_[147+t] + B01_current[t] * data_[126+t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[175+t] = C00_[t] * data_[168+t] + cB00_current[t] * data_[147+t];

  for (int t = 0; t != 7; ++t)
    data_[182+t] = C00_[t] * data_[175+t] + B10_[t] * data_[168+t] + cB00_current[t] * data_[154+t];

  for (int t = 0; t != 7; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[189+t] = D00_[t] * data_[168+t] + B01_current[t] * data_[147+t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[196+t] = C00_[t] * data_[189+t] + cB00_current[t] * data_[168+t];

  for (int t = 0; t != 7; ++t)
    data_[203+t] = C00_[t] * data_[196+t] + B10_[t] * data_[189+t] + cB00_current[t] * data_[175+t];

  for (int t = 0; t != 7; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 7; ++t)
    data_[210+t] = D00_[t] * data_[189+t] + B01_current[t] * data_[168+t];

  for (int t = 0; t != 7; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 7; ++t)
    data_[217+t] = C00_[t] * data_[210+t] + cB00_current[t] * data_[189+t];

  for (int t = 0; t != 7; ++t)
    data_[224+t] = C00_[t] * data_[217+t] + B10_[t] * data_[210+t] + cB00_current[t] * data_[196+t];
}

