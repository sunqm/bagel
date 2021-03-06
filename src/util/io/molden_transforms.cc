//
// BAGEL - Parallel electron correlation program.
// Filename: molden_transforms.cc
// Copyright (C) 2012 Shane Parker
//
// Author: Shane Parker <shane.parker@u.northwestern.edu>
// Maintainer: NU theory
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

//
// This was originally code found in src/integral/.../carsph_gen.cc
// but repurposed here for the molden section. At the moment, it is limited to at most
// f functions.
//

#include <src/util/math/comb.h>
#include <src/util/math/factorial.h>
#include <src/util/io/moldenin.h>

using namespace std;
using namespace bagel;

#define LARGE 32
#define LEND 5

const static Comb comb;
const static Factorial factorial;

void MoldenIn::compute_transforms() {
  const double one = 1.0;
  const double two = 2.0;
  const double quarter = 0.25;

  vector<pair<int, double>> s0(1, make_pair(0, one));
  vector<vector<pair<int, double>>> s1(1, s0);
  lmtuv_.push_back(s1);

  for (int l = 1; l != LEND; ++l) {

    map<int, int> mapping;
    int cnt = 0;
    for (int z = 0; z <= l; ++z) {
      for (int y = 0; y <= l - z; ++y) {
        const int x = l - y - z;
        if (x < 0) continue;
        const int key = x + y * LARGE + z * LARGE * LARGE;
        mapping.emplace(key, cnt);
        ++cnt;
      }
    }

    vector<vector<pair<int, double>>> mtuv;
    for (int n = 0; n != 2 * l + 1; ++n) {
      int m = l - (n / 2);
      if (n % 2 == 1) m *= -1;

      const int vm2 = m < 0 ? 1 : 0;
      const int absm = m > 0 ? m : -m;
      vector<pair<int, double>> tuv;

      const double Nlms = one / pow(two, absm) / factorial(l) * sqrt((m == 0 ? one : two) * factorial(l + absm) * factorial(l - absm));
      const int tmax = floor((l - absm) / 2.0);
      for (int t = 0; t <= tmax; ++t) {
        for (int u = 0; u <= t; ++u) {
          const int vmax = 2 * floor((absm - vm2) / 2.0) + vm2;
          for (int v2 = vm2; v2 <= vmax; v2 += 2) {
            assert((v2 - vm2) % 2 == 0);
            const double Clmtuv = pow(-one, t + ((v2 - vm2) / 2)) * pow(quarter, t)
                                * comb(l, t) * comb(l - t, absm + t)
                                * comb(t, u) * comb(absm, v2) * Nlms;
            const int xexp = 2 * t + absm - 2 * u - v2;
            const int yexp = 2 * u + v2;
            const int zexp = l - 2 * t - absm;
            double denom = one;
            map<int, int>::const_iterator current = mapping.find(xexp + yexp * LARGE + zexp * LARGE * LARGE);
            assert(current != mapping.end());
            const double coeff = Clmtuv / denom;
            tuv.push_back({current->second, coeff});
          }
        }
      }
      /* I have no idea if this will work for the general case, but it works up to d */
      double scale = 0.0;
      for(auto ituv = tuv.begin(); ituv != tuv.end(); ++ituv) {
        scale += ituv->second*ituv->second;
      }

      for(auto ituv = tuv.begin(); ituv != tuv.end(); ++ituv) {
        ituv->second /= scale;
      }

      mtuv.push_back(tuv);
    }


    lmtuv_.push_back(mtuv);
  }
}

vector<double> MoldenIn::transform_cart(vector<double> carts, int ang_l) {
   vector<vector<pair<int,double>>> mtuv = lmtuv_.at(ang_l);

   vector<double> out;
   for(auto& im : mtuv) {
     double value = 0.0;

     for(auto& ituv : im) {
        value += (ituv.second) * carts.at(ituv.first);
     }

     out.push_back(value);
   }

   return out;
}
