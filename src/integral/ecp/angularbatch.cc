//
// BAGEL - Parallel electron correlation program.
// Filename: angularbatch.cc
// Copyright (C) 2014 Toru Shiozaki
//
// Author: Hai-Anh Le <anh@u.northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// alxyz[1] later version.
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


#include <src/math/bessel.h>
#include <src/integral/ecp/wigner3j.h>
#include <src/integral/ecp/angularbatch.h>

using namespace bagel;
using namespace std;

double AngularBatch::integrate3SHs(std::array<std::pair<int, int>, 3> lm) const {

  const int l1 = lm[0].first;
  const int l2 = lm[1].first;
  const int l3 = lm[2].first;

  static Wigner3j wigner3j;
  const double w1 = wigner3j.lookup_wigner3j(l1, lm[0].second, l2, lm[1].second, l3, lm[2].second);
  const double w2 = wigner3j.lookup_wigner3j(l1, 0, l2, 0, l3, 0);
  const double coeff = sqrt(0.25 * (2*l1 + 1) * (2*l2 + 1) * (2*l3 + 1) / pi__);

  return coeff * w1 * w2;

}

double AngularBatch::integrate3USP(array<int, 3> xyz_exponents) const {

  const int i = xyz_exponents[0];
  const int j = xyz_exponents[1];
  const int k = xyz_exponents[2];

  if (i % 2 == 0 && j % 2 == 0 && k % 2 == 0) {
    Factorial fact;
    const double num = 2.0 * fact(i) * fact(j) * fact(k) * fact((i+j+k+2)/2);
    const double denom = fact(i/2) * fact(j/2) * fact(k/2) * fact(i+j+k+2);
    return 4.0 * pi__ * num / denom;
  } else {
    return 0.0;
  }

}

double AngularBatch::integrate2SH1USP(const std::pair<int, int> lm1, const std::pair<int, int> lm2, const std::array<int, 3> ijk) const {

  vector<pair<double, int>> usp;

  array<int, 2> lm = {lm1.first, lm1.second};
  shared_ptr<SphHarmonics> sphusp = make_shared<SphHarmonics>(lm);
  int cnt = 0;
  int nonzero = 0;
  for (int lz = 0; lz <= lm1.first; ++lz) {
    for (int ly = 0; ly <= lm1.first - lz; ++ly) {
      const int lx = lm1.first - lz - ly;
      const double coeff = sphusp->sph_to_USP(lx, ly);
      if (coeff != 0.0) {
        nonzero ++;
        pair<double, int> c_usp(coeff, cnt);
        usp.push_back(c_usp);
      }
      ++cnt;
    }
  }
  const int n1 = nonzero;

  lm = {lm2.first, lm2.second};
  sphusp = make_shared<SphHarmonics>(lm);
  cnt = 0;
  nonzero = 0;
  for (int lz = 0; lz <= lm2.first; ++lz) {
    for (int ly = 0; ly <= lm2.first - lz; ++ly) {
      const int lx = lm2.first - lz - ly;
      const double coeff = sphusp->sph_to_USP(lx, ly);
      if (coeff != 0.0) {
        nonzero ++;
        pair<double, int> c_usp(coeff, cnt);
        usp.push_back(c_usp);
      }
      ++cnt;
    }
  }
  const int n2 = nonzero;

  assert (n1 + n2 == usp.size());
  double ans = 0.0;
  for (int i = 0; i != n1; ++i) {
    for (int j = n1; j != n1 + n2; ++j) {
      const double coeff = usp[i].first * usp[j].first;
      array<int, 3> ki;
      int id = usp[i].second;
      int kz = 0;
      for (int lp1 = lm1.first + 1; lp1 != 0; --lp1) {
        if (id - lp1 < 0) {
          ki[2] = kz;
          ki[1] = id;
          ki[0] = lm1.first - ki[2] - ki[1];
          break;
        } else {
          kz++;
          id -= lp1;
        }
      }
      array<int, 3> kj;
      id = usp[j].second;
      kz = 0;
      for (int lp1 = lm2.first + 1; lp1 != 0; --lp1) {
        if (id - lp1 < 0) {
          kj[2] = kz;
          kj[1] = id;
          kj[0] = lm2.first - kj[2] - kj[1];
          break;
        } else {
          kz++;
          id -= lp1;
        }
      }
      const int x = ki[0] + kj[0] + ijk[0];
      const int y = ki[1] + kj[1] + ijk[1];
      const int z = ki[2] + kj[2] + ijk[2];
      array<int, 3> xyz = {x, y, z};
      ans += coeff * integrate3USP(xyz);
    }
  }

  return ans;

}


double AngularBatch::project_one_centre(array<double, 3> posA, const array<int, 3> lxyz, const double expA,
                                        array<double, 3> posB, const array<int, 2> lm, const double r) {

  Comb comb;
  array<double, 3> AB;
  for (int i = 0; i != 3; ++i) AB[i] = posA[i] - posB[i];
  const double dAB = sqrt(AB[0]*AB[0] + AB[1]*AB[1] + AB[2]*AB[2]);
  const int nu = lxyz[0] + lxyz[1] + lxyz[2];
  const int lnu = lm[0] + nu;
  const double exponential = exp(-expA * (dAB - r) * (dAB - r));
  double ans = 0.0;
  for (int kx = 0; kx <= lxyz[0]; ++kx) {
    const double ckx = comb(lxyz[0], kx) * pow(AB[0], lxyz[0] - kx);
    for (int ky = 0; ky <= lxyz[1]; ++ky) {
      const double cky = comb(lxyz[1], ky) * pow(AB[1], lxyz[1] - ky);
      for (int kz = 0; kz <= lxyz[2]; ++kz) {
        const double ckz = comb(lxyz[2], kz) * pow(AB[2], lxyz[2] - kz);
        const int lk = kx + ky + kz;
        const double rxyz = pow(r, lk);

        double sld = 0.0;
        for (int ld = 0; ld <= lnu; ++ld) {
          double smu = 0.0;
          for (int m = 0; m <= 2 * ld; ++m) {
            const int mu = m - ld;
            shared_ptr<SphHarmonics> sphAB = make_shared<SphHarmonics>(ld, mu, AB);
            const double Z_AB = (dAB == 0 ? (1.0/sqrt(4.0*pi__)) : sphAB->zlm());

            const array<int, 3> exp = {kx, ky, kz};
            const pair<int, int> lm1(ld, mu);
            const pair<int, int> lm2(lm[0], lm[1]);
            smu += Z_AB * integrate2SH1USP(lm1, lm2, exp);
          }
          MSphBesselI msbessel(ld);
          const double sbessel = msbessel.compute(2.0 * expA * dAB * r);
          sld += smu * sbessel;
        }
        ans += sld * ckx * cky * ckz * rxyz * pow(-1.0, lk - nu);
      }
    }
  }

  return ans * 4.0 * pi__ * exponential;

}

double AngularBatch::project_many_centres(const array<int, 3> lA, const double expA,
                                          const array<int, 3> lC, const double expC, const double r) {

 vector<shared_ptr<const Shell_ECP>> shells_ecp = ecp_->shells_ecp();

 double ans = 0.0;
 for (auto& ishecp : shells_ecp) {
   const int l = ishecp->angular_number();
   if (l != ecp_->maxl()) {
     for (int mu = 0; mu != 2*l; ++mu) {
       int index = 0;
       const int m = mu - l;
       array<int, 2> lm = {l, m};
       for (auto& exponents : ishecp->ecp_exponents()) {
         const double projA = project_one_centre(basisinfo_[0]->position(), lA, expA, ishecp->position(), lm, r);
         const double projC = project_one_centre(basisinfo_[1]->position(), lC, expC, ishecp->position(), lm, r);
         ans += ishecp->ecp_coefficients(index) * projA *
                std::pow(r, -ishecp->ecp_r_power(index)) * std::exp(-exponents * r * r) * r * r * projC;
         ++index;
       }
     }
   }
 }

 return ans;

}

#if 1
double AngularBatch::compute(const double r) {

  const vector<double> expA = basisinfo_[0]->exponents();
  const vector<double> expC = basisinfo_[1]->exponents();

  double ans = 0.0;

  for (auto& expiA : expA) {
    const int lA = basisinfo_[0]->angular_number();
    for (auto& expiC : expC) {
      const int lC = basisinfo_[1]->angular_number();

      for (int lzA = 0; lzA <= lA; ++lzA) {
        for (int lyA = 0; lyA <= lA - lzA; ++lzA) {
          const int lxA = lA - lzA - lyA;
          array<int, 3> lxyzA = {lxA, lyA, lzA};
          for (int lzC = 0; lzC <= lC; ++lzC) {
            for (int lyC = 0; lyC <= lC - lzC; ++lyC) {
              const int lxC = lC - lzC - lyC;
              array<int, 3> lxyzC = {lxC, lyC, lzC};
              ans += project_many_centres(lxyzA, expiA, lxyzC, expiC, r);
            }
          }
        }
      }

    }
  }

  return ans;
// TODO: coefficients

}
#endif
