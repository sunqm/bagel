//
// BAGEL - Parallel electron correlation program.
// Filename: gamma_tensor.h
// Copyright (C) 2014 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
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

#ifndef BAGEL_ASD_GAMMA_TENSOR_H
#define BAGEL_ASD_GAMMA_TENSOR_H

#include <src/asd/gamma_forest.h>
#include <src/asd/dimersubspace.h>

namespace bagel {

namespace asd {
  template <typename T, int N>
  struct Wrap {
    std::shared_ptr<T> f;
    Wrap(std::shared_ptr<T> p) : f(p) { }
  };
}

// Store Gamma Tensor as a block-sparse tensor.
class GammaTensor {
  public:
    struct listGammaSQ {
      std::list<GammaSQ> list;
      listGammaSQ(std::list<GammaSQ>& l) : list(l) { }
      listGammaSQ(std::list<GammaSQ>&& l) : list(l) { }

      bool operator==(const listGammaSQ& o) const { return list == o.list; }
      bool operator< (const listGammaSQ& o) const { return list <  o.list; }
      void print() const {
        for (auto& i : list) std::cout << i << " ";
        std::cout << std::endl;
      }
    };

  protected:
    using SparseMap = std::map<std::tuple<listGammaSQ, MonomerKey, MonomerKey>, std::shared_ptr<btas::Tensor3<double>>>;
    SparseMap sparse_;

    static const std::list<std::list<GammaSQ>> oplist_;

  public:
    // default constructor
    GammaTensor() { }

    // constructor that takes GammaForst.
    // *** CAUTION *** this constructor moves the data! (one can instead copy construct - if needed change the lines below).
    template <typename T, int N, int M>
    GammaTensor(asd::Wrap<GammaForest<T,N>,M> forest, const std::vector<DimerSubspace<T>>& sp) {
      std::shared_ptr<GammaForest<T,N>> f = forest.f;

      // initialize operator space
      for (auto o : oplist_) {
        for (auto& i : sp)
          for (auto& j : sp) {
            if (f->template exist<M>(i.template tag<M>(), j.template tag<M>(), o)) {
              std::shared_ptr<Matrix> mat = f->template get<M>(i.template tag<M>(), j.template tag<M>(), o);
              btas::CRange<3> range(i.template monomerkey<M>().nstates(), j.template monomerkey<M>().nstates(), mat->mdim());
              // checking ndim
              assert(mat->ndim() == range.extent(0)*range.extent(1));
              // checking mdim
              assert(mat->mdim() == range.extent(2));
              // internal check
              assert(mat->storage().size() == range.area());
              // convert this matrix to 3-tensor
              auto tensor = std::make_shared<btas::Tensor3<double>>(range, std::move(mat->storage()));
              sparse_.emplace(std::make_tuple(listGammaSQ(o), i.template monomerkey<M>(), j.template monomerkey<M>()), tensor);
            }
          }
      }
    }

    // copy constructor
    GammaTensor(const GammaTensor& o) {
      for (auto& i : o.sparse_)
        sparse_.emplace(i.first, std::make_shared<btas::Tensor3<double>>(*i.second));
    }
    // move constructor
    GammaTensor(GammaTensor&& o) {
      for (auto& i : o.sparse_)
        sparse_.emplace(i.first, i.second);
    }

    // copy and move assignment operators
    GammaTensor& operator=(const GammaTensor& o);
    GammaTensor& operator=(GammaTensor&& o);

    std::shared_ptr<GammaTensor> copy() const { return std::make_shared<GammaTensor>(*this); }
    std::shared_ptr<GammaTensor> clone() const {
      auto out = std::make_shared<GammaTensor>();
      for (auto& i : sparse_)
        out->emplace(i.first, std::make_shared<btas::Tensor3<double>>(i.second->range(), 0.0));
      return out;
    }

    auto begin() -> decltype(sparse_.begin()) { return sparse_.begin(); }
    auto end() -> decltype(sparse_.end()) { return sparse_.end(); }
    auto begin() const -> decltype(sparse_.cbegin()) { return sparse_.cbegin(); }
    auto end() const -> decltype(sparse_.cend()) { return sparse_.cend(); }
    auto cbegin() const -> decltype(sparse_.cbegin()) { return sparse_.cbegin(); }
    auto cend() const -> decltype(sparse_.cend()) { return sparse_.cend(); }

    int nblocks() const { return sparse_.size(); }
    bool exist(const std::tuple<listGammaSQ, MonomerKey, MonomerKey> tag) const { return sparse_.find(tag) != sparse_.end(); }
    bool exist(const MonomerKey& i, const MonomerKey& j, const std::initializer_list<GammaSQ>& o) const {
      return exist(std::make_tuple(std::list<GammaSQ>(o), i, j));
    }

    template <typename... Args>
    auto emplace(Args... args) -> decltype(sparse_.emplace(std::forward<Args>(args)...)) { return sparse_.emplace(std::forward<Args>(args)...); }

    std::shared_ptr<btas::Tensor3<double>> get_block(const std::tuple<listGammaSQ, MonomerKey, MonomerKey> tag) { return sparse_.at(tag); }
    std::shared_ptr<const btas::Tensor3<double>> get_block(const std::tuple<listGammaSQ, MonomerKey, MonomerKey> tag) const { return sparse_.at(tag); }
    std::shared_ptr<const btas::Tensor3<double>> get_block(const MonomerKey& i, const MonomerKey& j, const std::initializer_list<GammaSQ>& o) const {
      return sparse_.at(std::make_tuple(std::list<GammaSQ>(o), i, j));
    }

    MatView get_block_as_matview(const MonomerKey& i, const MonomerKey& j, const std::initializer_list<GammaSQ>& o) const {
      auto tensor = sparse_.at(std::make_tuple(std::list<GammaSQ>(o), i, j));
      btas::CRange<2> range(tensor->extent(0)*tensor->extent(1), tensor->extent(2));
      return MatView(btas::make_view(range, tensor->storage()), /*localized*/false);
    }
};

}

#endif
