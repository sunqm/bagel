//
// BAGEL - Parallel electron correlation program.
// Filename: _svrr_2060.cc
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

// returns double array of length 105
void SVRRList::_svrr_2060(double* data_, const double* C00_, const double* D00_, const double* B00_, const double* B01_, const double* B10_) {
  for (int t = 0; t != 5; ++t)
    data_[0+t] = 1.0;

  for (int t = 0; t != 5; ++t)
    data_[5+t] = C00_[t];

  for (int t = 0; t != 5; ++t)
    data_[10+t] = C00_[t] * data_[5+t] + B10_[t];

  for (int t = 0; t != 5; ++t)
    data_[15+t] = D00_[t];

  double cB00_current[5];
  for (int t = 0; t != 5; ++t)
    cB00_current[t] = B00_[t];

  for (int t = 0; t != 5; ++t)
    data_[20+t] = C00_[t] * data_[15+t] + cB00_current[t];

  for (int t = 0; t != 5; ++t)
    data_[25+t] = C00_[t] * data_[20+t] + B10_[t] * data_[15+t] + cB00_current[t] * data_[5+t];

  double B01_current[5];
  for (int t = 0; t != 5; ++t)
    B01_current[t] = B01_[t];

  for (int t = 0; t != 5; ++t)
    data_[30+t] = D00_[t] * data_[15+t] + B01_current[t];

  for (int t = 0; t != 5; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 5; ++t)
    data_[35+t] = C00_[t] * data_[30+t] + cB00_current[t] * data_[15+t];

  for (int t = 0; t != 5; ++t)
    data_[40+t] = C00_[t] * data_[35+t] + B10_[t] * data_[30+t] + cB00_current[t] * data_[20+t];

  for (int t = 0; t != 5; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 5; ++t)
    data_[45+t] = D00_[t] * data_[30+t] + B01_current[t] * data_[15+t];

  for (int t = 0; t != 5; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 5; ++t)
    data_[50+t] = C00_[t] * data_[45+t] + cB00_current[t] * data_[30+t];

  for (int t = 0; t != 5; ++t)
    data_[55+t] = C00_[t] * data_[50+t] + B10_[t] * data_[45+t] + cB00_current[t] * data_[35+t];

  for (int t = 0; t != 5; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 5; ++t)
    data_[60+t] = D00_[t] * data_[45+t] + B01_current[t] * data_[30+t];

  for (int t = 0; t != 5; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 5; ++t)
    data_[65+t] = C00_[t] * data_[60+t] + cB00_current[t] * data_[45+t];

  for (int t = 0; t != 5; ++t)
    data_[70+t] = C00_[t] * data_[65+t] + B10_[t] * data_[60+t] + cB00_current[t] * data_[50+t];

  for (int t = 0; t != 5; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 5; ++t)
    data_[75+t] = D00_[t] * data_[60+t] + B01_current[t] * data_[45+t];

  for (int t = 0; t != 5; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 5; ++t)
    data_[80+t] = C00_[t] * data_[75+t] + cB00_current[t] * data_[60+t];

  for (int t = 0; t != 5; ++t)
    data_[85+t] = C00_[t] * data_[80+t] + B10_[t] * data_[75+t] + cB00_current[t] * data_[65+t];

  for (int t = 0; t != 5; ++t)
    B01_current[t] += B01_[t];

  for (int t = 0; t != 5; ++t)
    data_[90+t] = D00_[t] * data_[75+t] + B01_current[t] * data_[60+t];

  for (int t = 0; t != 5; ++t)
    cB00_current[t] += B00_[t];

  for (int t = 0; t != 5; ++t)
    data_[95+t] = C00_[t] * data_[90+t] + cB00_current[t] * data_[75+t];

  for (int t = 0; t != 5; ++t)
    data_[100+t] = C00_[t] * data_[95+t] + B10_[t] * data_[90+t] + cB00_current[t] * data_[80+t];
}

