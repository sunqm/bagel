//
// BAGEL - Parallel electron correlation program.
// Filename: atomicdensities.h
// Copyright (C) 2013 Toru Shiozaki
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

#ifndef __SRC_SCF_ATOMICDENSITIES_H
#define __SRC_SCF_ATOMICDENSITIES_H

#include <src/wfn/geometry.h>

namespace bagel {

class AtomicDensities : public Matrix {
  protected:
    std::shared_ptr<const Geometry> geom_;

    std::shared_ptr<const Matrix> compute_atomic(std::shared_ptr<const Geometry> ga) const;

  public:
    AtomicDensities(std::shared_ptr<const Geometry> g);

};

}

#endif
