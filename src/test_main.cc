//
// Newint - Parallel electron correlation program.
// Filename: test_main.cc
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


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Suites
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <memory>
#include <stdexcept>
#include <src/stackmem.h>
StackMem* stack;

static double THRESH = 1.0e-8;
 
bool compare(const double a, const double b, const double thr = THRESH) { return fabs(a-b) < thr; };
bool compare(const std::vector<double> a, const std::vector<double> b, const double thr = THRESH) {
 if (a.size() != b.size()) throw std::logic_error("comparing vectors with different sizes");
 bool out = true;
 for (auto i = a.begin(), j = b.begin(); i != a.end(); ++i, ++j) out &= compare(*i, *j, thr);
 return out; 
};

#include <src/scf/test_scf.cc>

#include <src/mp2/test_mp2.cc>

#include <src/fci/test_fci.cc>

#include <src/opt/test_opt.cc>
