//
// BAGEL - Parallel electron correlation program.
// Filename: relmofile.h
// Copyright (C) 2013 Toru Shiozaki
//
// Author: Michael Caldwell  <caldwell@u.northwestern.edu>
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


#ifndef __BAGEL_ZFCI_RELMOFILE_H
#define __BAGEL_ZFCI_RELMOFILE_H

#include <unordered_map>
#include <src/util/math/zmatrix.h>
#include <src/df/reldffull.h>
#include <src/scf/dhf/dfock.h>
#include <src/wfn/relreference.h>

namespace bagel {

class RelMOFile {
  protected:
    int nocc_;
    int nbasis_;
    int charge_;
    double core_energy_;

    std::shared_ptr<const Geometry> geom_;
    std::shared_ptr<const ZMatrix> core_fock_;
    std::shared_ptr<const ZMatrix> coeff_;
    std::array<std::shared_ptr<const ZMatrix>,2> kramers_coeff_;

    bool gaunt_;
    bool breit_;
    bool tsymm_;

    // creates integral files and returns the core energy.
    void init(const int nstart, const int nend, const bool restricted = false);

    // hamiltoniam data
    std::unordered_map<std::bitset<2>, std::shared_ptr<const ZMatrix>> mo1e_;
    std::unordered_map<std::bitset<4>, std::shared_ptr<const ZMatrix>> mo2e_;

    // generates Kramers symmetry-adapted orbitals
    void compress_and_set(std::unordered_map<std::bitset<2>, std::shared_ptr<const ZMatrix>> buf1e,
                          std::unordered_map<std::bitset<4>, std::shared_ptr<const ZMatrix>> buf2e);

    virtual std::unordered_map<std::bitset<2>, std::shared_ptr<const ZMatrix>> compute_mo1e(const std::array<std::shared_ptr<const ZMatrix>,2> coeff) = 0;
    virtual std::unordered_map<std::bitset<4>, std::shared_ptr<const ZMatrix>> compute_mo2e(const std::array<std::shared_ptr<const ZMatrix>,2> coeff) = 0;

    // half transformed integrals for CASSCF
    std::array<std::list<std::shared_ptr<RelDFHalf>>,2> half_complex_coulomb_;
    std::array<std::list<std::shared_ptr<RelDFHalf>>,2> half_complex_gaunt_;

  public:
    RelMOFile(const std::shared_ptr<const Geometry>, std::shared_ptr<const ZMatrix>, const int charge, const bool gaunt, const bool breit, const bool tsymm);

    // static function
    static std::array<std::shared_ptr<const ZMatrix>,2> kramers(std::shared_ptr<const ZMatrix> coeff, std::shared_ptr<const ZMatrix> overlap, std::shared_ptr<const ZMatrix> eig);
    std::array<std::shared_ptr<const ZMatrix>,2> kramers_zquat(const int nstart, const int nfence, std::shared_ptr<const ZMatrix> coeff, std::shared_ptr<const ZMatrix> overlap, std::shared_ptr<const ZMatrix> hcore);

    // static function used to make the order of eigenvalues & eigenvectors of ZMatrix::diagonalize() match that given by QuatMatrix::diagonalize()
    static void rearrange_eig(VectorB& eig, std::shared_ptr<ZMatrix> coeff, const bool includes_neg = true);

    std::shared_ptr<const ZMatrix> core_fock() const { return core_fock_; }

    std::shared_ptr<const ZMatrix> mo1e(const std::bitset<2>& b) const { return mo1e_.at(b); }
    std::shared_ptr<const ZMatrix> mo2e(const std::bitset<4>& b) const { return mo2e_.at(b); }
    const std::complex<double>& mo1e(const std::bitset<2>& b, const size_t i, const size_t j) const { return mo1e_.at(b)->element(i,j); }
    const std::complex<double>& mo2e(const std::bitset<4>& b, const size_t i, const size_t j, const size_t k, const size_t l) const { return mo2e_.at(b)->element(i+nocc_*j, k+nocc_*l); }
    std::shared_ptr<const ZMatrix> mo1e(std::string&& b) const { return mo1e(std::bitset<2>(std::move(b))); }
    std::shared_ptr<const ZMatrix> mo2e(std::string&& b) const { return mo2e(std::bitset<4>(std::move(b))); }
    const std::complex<double>& mo1e(std::string&& b, const size_t i, const size_t j) const { return mo1e(std::bitset<2>(std::move(b)), i, j); }
    const std::complex<double>& mo2e(std::string&& b, const size_t i, const size_t j, const size_t k, const size_t l) const { return mo2e(std::bitset<4>(std::move(b)), i, j, k, l); }
    std::unordered_map<std::bitset<4>, std::shared_ptr<const ZMatrix>> mo2e() const { return mo2e_; }

    double core_energy() const { return core_energy_; }

    std::shared_ptr<const ZMatrix> kramers_coeff(const int i) const { return kramers_coeff_[i]; }
    std::array<std::shared_ptr<const ZMatrix>,2> kramers_coeff() const { return kramers_coeff_; }
    std::shared_ptr<const ZMatrix> coeff() const {
      auto coeff_tot = std::make_shared<ZMatrix>(kramers_coeff_[0]->ndim(), nocc_*2);
      coeff_tot->copy_block(0,     0, kramers_coeff_[0]->ndim(), nocc_, kramers_coeff_[0]);
      coeff_tot->copy_block(0, nocc_, kramers_coeff_[1]->ndim(), nocc_, kramers_coeff_[1]);
      return coeff_tot;
    }
    std::shared_ptr<const ZMatrix> coeff_input() const { return coeff_; }
    void update_kramers_coeff(std::shared_ptr<ZMatrix> coeff);
    std::array<std::list<std::shared_ptr<RelDFHalf>>,2> half_complex_coulomb() const { return half_complex_coulomb_; }
    std::array<std::list<std::shared_ptr<RelDFHalf>>,2> half_complex_gaunt() const { return half_complex_gaunt_; }


    static std::unordered_map<std::bitset<2>, std::shared_ptr<const RelDFFull>>
        compute_full(std::array<std::array<std::shared_ptr<const Matrix>,4>,2> rocoeff, std::array<std::array<std::shared_ptr<const Matrix>,4>,2> iocoeff,
                     std::array<std::list<std::shared_ptr<RelDFHalf>>,2> half, const bool appj, const bool appjj = false);
};


class RelJop : public RelMOFile {
  protected:
    std::unordered_map<std::bitset<2>, std::shared_ptr<const ZMatrix>> compute_mo1e(const std::array<std::shared_ptr<const ZMatrix>,2> coeff) override;
    std::unordered_map<std::bitset<4>, std::shared_ptr<const ZMatrix>> compute_mo2e(const std::array<std::shared_ptr<const ZMatrix>,2> coeff) override;

  public:
    RelJop(const std::shared_ptr<const Geometry> geo, const int c, const int d, std::shared_ptr<const ZMatrix> coeff, const int charge,
      const bool gaunt, const bool breit, const bool restricted = false, const bool tsymm = true)
      : RelMOFile(geo, coeff, charge, gaunt, breit, tsymm) { init(c, d, restricted); }
};

}

#endif
