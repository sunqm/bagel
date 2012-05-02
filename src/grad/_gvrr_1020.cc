//
// Newint - Parallel electron correlation program.
// Filename: _gvrr_1020.cc
// Copyright (C) 2009 Toru Shiozaki
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


#include "gvrrlist.h"

// returns double array of length 12
void GVRRList::_gvrr_1020(double* data_, const double* C00_, const double* D00_, const double* B00_, const double* B01_, const double* B10_) {
  data_[0] = 1.0;
  data_[1] = 1.0;

  data_[2] = C00_[0];
  data_[3] = C00_[1];

  data_[4] = D00_[0];
  data_[5] = D00_[1];

  double cB00_current[2];
  cB00_current[0] = B00_[0];
  cB00_current[1] = B00_[1];

  data_[6] = C00_[0] * data_[4] + cB00_current[0];
  data_[7] = C00_[1] * data_[5] + cB00_current[1];


  data_[8] = D00_[0] * data_[4] + B01_[0];
  data_[9] = D00_[1] * data_[5] + B01_[1];

  cB00_current[0] += B00_[0];
  cB00_current[1] += B00_[1];

  data_[10] = C00_[0] * data_[8] + cB00_current[0] * data_[4];
  data_[11] = C00_[1] * data_[9] + cB00_current[1] * data_[5];
}

