//
// BAGEL - Parallel electron correlation program.
// Filename: dipolematrix.cc
// Copyright (C) 2013 Toru Shiozaki
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


#include <src/mat1e/dipolematrix.h>
#include <src/integral/os/mmbatch.h>

using namespace std;
using namespace bagel;

DipoleMatrix::DipoleMatrix(const shared_ptr<const Molecule> mol) : Matrix1eArray<3>(mol) {
  init(mol);
  fill_upper();
}

void DipoleMatrix::computebatch(const array<shared_ptr<const Shell>,2>& input, const int offsetb0, const int offsetb1, shared_ptr<const Molecule> mol) {
  // input = [b1, b0]
  const int dimb1 = input[0]->nbasis();
  const int dimb0 = input[1]->nbasis();
  DipoleBatch dipole(input, mol);
  dipole.compute();

  for (int i = 0; i < Nblocks(); ++i) {
    matrices_[i]->copy_block(offsetb1, offsetb0, dimb1, dimb0, dipole.data(i));
  }
}
