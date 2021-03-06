//
// BAGEL - Parallel electron correlation program.
// Filename: gamma_forest.h
// Copyright (C) 2013 Shane Parker
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

#ifndef __asd_gamma_forest_h
#define __asd_gamma_forest_h

#include <set>
#include <src/ci/fci/dvec.h>
#include <src/ci/ras/civector.h>
#include <src/util/math/matrix.h>
#include <src/util/taskqueue.h>
#include <src/asd/coupling.h>
#include <src/asd/dimersubspace.h>
#include <src/asd/gamma_sq.h>

namespace bagel {

template <typename VecType>
class GammaBranch {
  protected:
    std::array<std::shared_ptr<GammaBranch<VecType>>, 4> branches_;
    std::map<size_t, std::shared_ptr<const VecType>> bras_; // use tags as unique identifier
    std::map<size_t, std::shared_ptr<Matrix>> gammas_;

    bool active_;

  public:
    GammaBranch() : active_(false) {}

    void insert(std::shared_ptr<const VecType> bra, const size_t bra_tag, const std::list<GammaSQ>& gsq) {
      if (gsq.empty()) {
        bras_.emplace(bra_tag, bra);
      } else {
        auto first = gsq.back();
        auto rest = gsq; rest.pop_back();
        std::shared_ptr<GammaBranch<VecType>> target = branches_[static_cast<int>(first)];

        target->activate();
        target->insert(bra, bra_tag, rest);
      }
    }

    std::shared_ptr<const Matrix> search(const size_t tag, const std::list<GammaSQ>& gsq) const {
      if (gsq.empty()) {
        assert(gammas_.find(tag)!=gammas_.end()); return gammas_.find(tag)->second;
      } else {
        auto first = gsq.back();
        auto rest = gsq; rest.pop_back();
        return branch(first)->search(tag, rest);
      }
    }

    std::shared_ptr<Matrix> search(const size_t tag, const std::list<GammaSQ>& gsq) {
      if (gsq.empty()) {
        assert(gammas_.find(tag)!=gammas_.end()); return gammas_.find(tag)->second;
      } else {
        auto first = gsq.back();
        auto rest = gsq; rest.pop_back();
        return branch(first)->search(tag, rest);
      }
    }

    bool exist(const size_t tag, const std::list<GammaSQ>& gsq) const {
      if (gsq.empty())
        return gammas_.find(tag) != gammas_.end();
      else {
        auto first = gsq.back();
        auto rest = gsq; rest.pop_back();
        return branch(first) ? branch(first)->exist(tag, rest) : false;
      }
    }

    void activate() { active_ = true; }
    bool active() const { return active_; }

    std::shared_ptr<GammaBranch<VecType>>& branch(const int i) { return branches_[i]; }
    const std::shared_ptr<GammaBranch<VecType>>& branch(const int i) const { return branches_[i]; }
    std::shared_ptr<GammaBranch<VecType>>& branch(const GammaSQ sq) { return branch(static_cast<int>(sq)); }
    const std::shared_ptr<GammaBranch<VecType>>& branch(const GammaSQ sq) const { return branch(static_cast<int>(sq)); }

    const std::map<size_t, std::shared_ptr<const VecType>>& bras() const { return bras_; }
    std::map<size_t, std::shared_ptr<Matrix>>& gammas() { return gammas_; }

    bool if_contributes(std::set<int> needed) {
      bool contributes = false;
      for (const int& b : needed) {
        if (branch(b)) contributes |= branch(b)->active();
      }
      if (!contributes) {
        for (int i = 0; i < 4; ++i) {
          if (branch(i)) {
            if (branch(i)->active()) contributes |= branch(i)->if_contributes(needed);
          }
        }
      }
      return contributes;
    }

    // distterminal means there are no alpha operators higher in the tree
    bool is_distterminal() {
      return false;
#if 0
      bool not_terminal = false;
      if (branch(GammaSQ::CreateAlpha))
        not_terminal |= branch(GammaSQ::CreateAlpha)->active();
      if (branch(GammaSQ::AnnihilateAlpha))
        not_terminal |= branch(GammaSQ::AnnihilateAlpha)->active();
      if (!not_terminal) {
        if (branch(GammaSQ::CreateBeta) && branch(GammaSQ::CreateBeta)->active())
          not_terminal |= branch(GammaSQ::CreateBeta)->is_distterminal();
        if (branch(GammaSQ::AnnihilateBeta) && branch(GammaSQ::AnnihilateBeta)->active())
          not_terminal |= branch(GammaSQ::AnnihilateBeta)->is_distterminal();
      }
      return !not_terminal;
#endif
    }

};

template <typename VecType>
class GammaTree {
  protected:
    std::shared_ptr<const VecType> ket_;
    std::shared_ptr<GammaBranch<VecType>> base_;

  public:
    GammaTree(std::shared_ptr<const VecType> ket) : ket_(ket) {
      base_ = std::make_shared<GammaBranch<VecType>>();
      constexpr int nops = 4;

      for (int i = 0; i < nops; ++i) {
        base_->branch(i) = std::make_shared<GammaBranch<VecType>>();
        for (int j = 0; j < nops; ++j) {
          base_->branch(i)->branch(j) = std::make_shared<GammaBranch<VecType>>();
          for (int k = 0; k < nops; ++k) {
            base_->branch(i)->branch(j)->branch(k) = std::make_shared<GammaBranch<VecType>>();
          }
        }
      }
    }

    std::shared_ptr<GammaBranch<VecType>> base() { return base_; }

    void insert(std::shared_ptr<const VecType> bra, const size_t tag, const std::list<GammaSQ>& ops) { base_->insert(bra, tag, ops); }
    std::shared_ptr<      Matrix> search(const size_t tag, const std::list<GammaSQ>& address)       { return base_->search(tag, address); }
    std::shared_ptr<const Matrix> search(const size_t tag, const std::list<GammaSQ>& address) const { return base_->search(tag, address); }
    bool exist(const size_t tag, const std::list<GammaSQ>& address) const { return base_->exist(tag, address); }

    std::shared_ptr<const VecType> ket() const { return ket_; }

    int norb() const { return ket()->det()->norb(); }
};

template <typename VecType>
class GammaTask {
  protected:
    const int a_;                            // Orbital
    const GammaSQ  operation_;               // Which operation
    const std::shared_ptr<GammaTree<VecType>> tree_;  // destination

  public:
    GammaTask(const std::shared_ptr<GammaTree<VecType>> tree, const GammaSQ operation, const int a) : a_(a), operation_(operation), tree_(tree) {}

    void compute() {
      constexpr int nops = 4;
      const int norb = tree_->norb();

      auto action = [] (const int op) { return is_creation(GammaSQ(op)); };
      auto spin = [] (const int op) { return is_alpha(GammaSQ(op)); };

      std::shared_ptr<GammaBranch<VecType>> first = tree_->base()->branch(operation_);
      assert(first->active()); // This should have been checked before sending it to the TaskQueue

      std::shared_ptr<const VecType> avec = tree_->ket()->apply(a_, action(static_cast<int>(operation_)), spin(static_cast<int>(operation_)));
      for (auto& ibra : first->bras())
        dot_product(ibra.second, avec, first->gammas().find(ibra.first)->second->element_ptr(0,a_));

      for (int j = 0; j < nops; ++j) {
        auto second = first->branch(j);
        if (!second->active()) continue;

        for (int b = 0; b < norb; ++b) {
          if (b==a_ && j==static_cast<int>(operation_)) continue;
          std::shared_ptr<const VecType> bvec = avec->apply(b, action(j), spin(j));
          for (auto& jbra : second->bras())
            dot_product(jbra.second, bvec, second->gammas().find(jbra.first)->second->element_ptr(0, a_*norb + b));

          for (int k = 0; k < nops; ++k) {
            std::shared_ptr<GammaBranch<VecType>> third = second->branch(k);
            if (!third->active()) continue;

            for (int c = 0; c < norb; ++c) {
              if (b==c && k==j) continue;
              std::shared_ptr<const VecType> cvec = bvec->apply(c, action(k), spin(k));
              for (auto& kbra : third->bras())
                dot_product(kbra.second, cvec, third->gammas().find(kbra.first)->second->element_ptr(0, a_*norb*norb + b*norb + c));
            }
          }
        }
      }
    }

    private:
      void dot_product(std::shared_ptr<const VecType> bras, std::shared_ptr<const VecType> kets, double* target) const {
        const int nbras = bras->ij();
        const int nkets = kets->ij();

        for (int iket = 0; iket < nkets; ++iket) {
          for (int jbra = 0; jbra < nbras; ++jbra, ++target) {
            *target = bras->data(jbra)->dot_product(*kets->data(iket));
          }
        }
      }
};


template <typename VecType, int N>
class GammaForest {
  protected:
    std::array<std::map<size_t, std::shared_ptr<GammaTree<VecType>>>, N> forests_;

  public:
    GammaForest() {}

    template <int unit>
    void insert(std::shared_ptr<const VecType> bra, const size_t bra_tag, std::shared_ptr<const VecType> ket, const size_t ket_tag, const std::list<GammaSQ>& ops) {
      std::shared_ptr<GammaTree<VecType>> gtree = tree<unit>(ket, ket_tag);
      gtree->insert(bra, bra_tag, ops);
    }

    template <int unit>
    std::shared_ptr<Matrix> get(const size_t bra_tag, const size_t ket_tag, const std::list<GammaSQ>& ops) {
      auto itree = forests_[unit].find(ket_tag); assert(itree!=forests_[unit].end());
      return itree->second->search(bra_tag, ops);
    }

    template <int unit>
    std::shared_ptr<const Matrix> get(const size_t bra_tag, const size_t ket_tag, const std::list<GammaSQ>& ops) const {
      auto itree = forests_[unit].find(ket_tag); assert(itree!=forests_[unit].end());
      return itree->second->search(bra_tag, ops);
    }

    template <int unit>
    bool exist(const size_t bra_tag, const size_t ket_tag, const std::list<GammaSQ>& ops) const {
      auto itree = forests_[unit].find(ket_tag);
      if (itree == forests_[unit].end())
        return false;
      else
        return itree->second->exist(bra_tag, ops);
    }

    template<class Func>
    void for_each_branch(Func func) {
      constexpr int nops = 4;
      for (auto& iforest : forests_) {
        for (auto& itreemap : iforest) {
          std::shared_ptr<GammaTree<VecType>> itree = itreemap.second;

          // Allocation sweep
          for (int i = 0; i < nops; ++i) {
            std::shared_ptr<GammaBranch<VecType>> first = itree->base()->branch(i);
            if (first->active()) func(first);

            for (int j = 0; j < nops; ++j) {
              std::shared_ptr<GammaBranch<VecType>> second = first->branch(j);
              if (second->active()) func(second);

              for (int k = 0; k < nops; ++k) {
                std::shared_ptr<GammaBranch<VecType>> third = second->branch(k);
                if (third->active()) func(third);
              }
            }
          }
        }
      }
    }

    int allocate_and_count() {
      constexpr int nops = 4;

      int ntasks = 0;
      // Allocate memory while counting tasks
      for (auto& iforest : forests_) {
        for (auto& itreemap : iforest) {
          std::shared_ptr<GammaTree<VecType>> itree = itreemap.second;
          const int nA = itree->ket()->ij();
          const int norb = itree->norb();
          for (auto& basebra : itree->base()->bras())
            itree->base()->gammas().emplace(basebra.first, std::make_shared<Matrix>(nA*basebra.second->ij(), 1));

          // Allocation sweep
          for (int i = 0; i < nops; ++i) {
            std::shared_ptr<GammaBranch<VecType>> first = itree->base()->branch(i);
            if (!first->active()) continue;
            ++ntasks;
            for (auto& ibra : first->bras()) {
              const int nAp = ibra.second->ij();
              const int nstates = nA * nAp;
              first->gammas().emplace(ibra.first, std::make_shared<Matrix>(nstates, norb));
            }

            for (int j = 0; j < nops; ++j) {
              std::shared_ptr<GammaBranch<VecType>> second = first->branch(j);
              if (!second->active()) continue;
              for (auto& jbra : second->bras()) {
                const int nAp = jbra.second->ij();
                const int nstates = nA * nAp;
                second->gammas().emplace(jbra.first, std::make_shared<Matrix>(nstates, norb * norb));
              }

              for (int k = 0; k < nops; ++k) {
                std::shared_ptr<GammaBranch<VecType>> third = second->branch(k);
                if (!third->active()) continue;
                for (auto& kbra : third->bras()) {
                  const int nAp = kbra.second->ij();
                  const int nstates = nA * nAp;
                  third->gammas().emplace(kbra.first, std::make_shared<Matrix>(nstates, norb * norb * norb));
                }
              }
            }
          }
        }
      }

      return ntasks;
    }

    void compute() {
      constexpr int nops = 4;

      const int ntasks = allocate_and_count();
      TaskQueue<GammaTask<VecType>> tasks(ntasks);

      // Add tasks
      for (auto& iforest : forests_) {
        for (auto& itreemap : iforest) {
          std::shared_ptr<GammaTree<VecType>> itree = itreemap.second;
          const int nket = itree->ket()->ij();
          for (auto& brapair : itree->base()->bras()) {
            double* target = itree->base()->gammas().at(brapair.first)->data();
            const int nbra = brapair.second->ij();
            for (int k = 0; k < nket; ++k)
              for (int b = 0; b < nbra; ++b, ++target)
                *target = brapair.second->data(b)->dot_product(*itree->ket()->data(k));
          }

          const int norb = itree->norb();
          for (int i = 0; i < nops; ++i) {
            std::shared_ptr<GammaBranch<VecType>> first = itree->base()->branch(i);
            if (!first->active()) continue;
            for (int a = 0; a < norb; ++a) tasks.emplace_back(itree, GammaSQ(i), a);
          }
        }
      }

      tasks.compute();
    }

    void couple_blocks(const DimerSubspace<VecType>& AB, const DimerSubspace<VecType>& ABp); // implemented in gamma_coupling.hpp

  private:
    template <int unit>
    std::shared_ptr<GammaTree<VecType>> tree(std::shared_ptr<const VecType> ket, const size_t ket_tag) {
      typename std::map<size_t, std::shared_ptr<GammaTree<VecType>>>::iterator itree = forests_[unit].find(ket_tag);
      if (itree == forests_[unit].end()) {
        forests_[unit].emplace(ket_tag, std::make_shared<GammaTree<VecType>>(ket));
        itree = forests_[unit].find(ket_tag);
      }
      return itree->second;
    }
};

#define ASD_HEADERS
#include <src/asd/gamma_coupling.hpp>
#undef ASD_HEADERS

template <>
void GammaForest<DistDvec, 2>::compute();

template <>
void GammaForest<DistRASDvec, 2>::compute();

template <class Branch>
class RASTask {
  protected:
    const int holes_;
    const int particles_;

  public:
    RASTask(const int h, const int p) : holes_(h), particles_(p) {}
    virtual std::shared_ptr<const RASString> stringspace(const int, const int, const int, const int , const int, const int) = 0;

    std::shared_ptr<RASBlock<double>> next_block(std::shared_ptr<Branch> branch, std::shared_ptr<const RASBlock<double>> base_block,
                                                 const int& orbital, const bool& action, const bool& spin) {
      std::shared_ptr<const RASString> sa = base_block->stringsa();
      std::shared_ptr<const RASString> sb = base_block->stringsb();

      std::array<int, 3> ras{{sa->ras<0>().second, sa->ras<1>().second, sa->ras<2>().second}};

      const int ras_space = ( orbital >= ras[0] ) + ( orbital >= (ras[0]+ras[1]) );
      std::array<int, 6> info{{ras[0] - sa->nholes(), ras[0] - sb->nholes(), sa->nele2(), sb->nele2(), sa->nparticles(), sb->nparticles()}};

      const int mod = action ? +1 : -1;
      info[2*ras_space]   += spin ? mod : 0;
      info[2*ras_space+1] += spin ? 0 : mod;

      // make sure it is a valid result
      for (int i = 0; i < 6; ++i)
        if (info[i] < 0 || info[i] > ras[i/2]) return nullptr;

      // is it out of space?
      const int nholes = 2*ras[0] - (info[0] + info[1]);
      const int nparts = info[4] + info[5];
      if (nholes > holes_ || nparts > particles_) {
        // peek ahead along the branch to see whether it will come back into the space

        // set of operations that would bring state back
        std::set<int> needed;
        if (nholes == (holes_+1)) {
          if (info[0] == ras[0]) {
            // no alpha holes, only beta holes
            needed.insert(static_cast<int>(GammaSQ::CreateBeta));
          }
          else if (info[1] == ras[0]) {
            // no beta holes, only alpha holes
            needed.insert(static_cast<int>(GammaSQ::CreateAlpha));
          }
          else {
            // both types of holes present
            needed.insert(static_cast<int>(GammaSQ::CreateAlpha));
            needed.insert(static_cast<int>(GammaSQ::CreateBeta));
          }
        }
        else if (nparts == (particles_+1)) {
          if (info[4] == 0) {
            // no alpha particles, only beta particles
            needed.insert(static_cast<int>(GammaSQ::AnnihilateBeta));
          }
          else if (info[5] == 0) {
            // no beta particles, only alpha particles
            needed.insert(static_cast<int>(GammaSQ::AnnihilateAlpha));
          }
          else {
            // both present
            needed.insert(static_cast<int>(GammaSQ::AnnihilateBeta));
            needed.insert(static_cast<int>(GammaSQ::AnnihilateAlpha));
          }
        }
        else {
          // impossible to contribute
          return nullptr;
        }

        // search down branch for active branches with needed operations
        if (!branch->if_contributes(needed))
          return nullptr;
      }

      std::shared_ptr<const RASString> ta = spin ? stringspace(info[0], ras[0], info[2], ras[1], info[4], ras[2]) : sa;
      std::shared_ptr<const RASString> tb = spin ? sb : stringspace(info[1], ras[0], info[3], ras[1], info[5], ras[2]);

      auto out = std::make_shared<RASBlock_alloc<double>>(ta,tb);

      RAS::Apply_block apply_block(orbital, action, spin);
      apply_block(base_block, out, branch->is_distterminal());

      return out;
    }
};

template <>
class GammaTask<RASDvec> : public RASTask<GammaBranch<RASDvec>> {
  protected:
    const int a_;                                     // Orbital
    const GammaSQ  operation_;                        // Which operation
    const std::shared_ptr<GammaTree<RASDvec>> tree_;  // destination

    // to avoid rebuilding the stringspaces repeatedly
    std::map<std::tuple<int, int, int, int, int, int>, std::shared_ptr<const RASString>> stringspaces_;

  public:
    GammaTask(const std::shared_ptr<GammaTree<RASDvec>> tree, const GammaSQ operation, const int a)
              : RASTask<GammaBranch<RASDvec>>(tree->ket()->det()->max_holes(), tree->ket()->det()->max_particles()),
                a_(a), operation_(operation), tree_(tree) {}

    void compute() {
      constexpr int nops = 4;
      const int norb = tree_->norb();

      auto action = [] (const int op) { return is_creation(GammaSQ(op)); };
      auto spin = [] (const int op) { return is_alpha(GammaSQ(op)); };

      std::shared_ptr<GammaBranch<RASDvec>> first = tree_->base()->branch(operation_);
      assert(first->active()); // This should have been checked before sending it to the TaskQueue

      std::shared_ptr<const RASDeterminants> base_det = tree_->ket()->det();

      const int nkets = tree_->ket()->ij();
      for (int iket = 0; iket < nkets; ++iket) {
        std::shared_ptr<const RASCivec> ketvec = tree_->ket()->data(iket);
        for (auto& ketblock : ketvec->blocks()) {
          if (!ketblock) continue;
          std::shared_ptr<const RASBlock<double>> ablock
            = next_block(first, ketblock, a_, action(static_cast<int>(operation_)), spin(static_cast<int>(operation_)));
          if (!ablock) continue;

          for (auto& ibra : first->bras())
            dot_product(ibra.second, ablock, first->gammas().find(ibra.first)->second->element_ptr(iket*ibra.second->ij(), a_));

          for (int j = 0; j < nops; ++j) {
            auto second = first->branch(j);
            if (!second->active()) continue;

            for (int b = 0; b < norb; ++b) {
              if (b==a_ && j==static_cast<int>(operation_)) continue;
              std::shared_ptr<const RASBlock<double>> bblock = next_block(second, ablock, b, action(j), spin(j));
              if (!bblock) continue;

              for (auto& jbra : second->bras())
                dot_product(jbra.second, bblock, second->gammas().find(jbra.first)->second->element_ptr(iket*jbra.second->ij(), a_*norb + b));

              for (int k = 0; k < nops; ++k) {
                std::shared_ptr<GammaBranch<RASDvec>> third = second->branch(k);
                if (!third->active()) continue;

                for (int c = 0; c < norb; ++c) {
                  if (b==c && k==j) continue;
                  std::shared_ptr<const RASBlock<double>> cblock = next_block(third, bblock, c, action(k), spin(k));
                  if (!cblock) continue;
                  for (auto& kbra : third->bras())
                    dot_product(kbra.second, cblock, third->gammas().find(kbra.first)->second->element_ptr(iket*kbra.second->ij(), a_*norb*norb + b*norb + c));
                }
              }
            }
          }
        }
      }
    }

    private:
      void dot_product(std::shared_ptr<const RASDvec> bras, std::shared_ptr<const RASBlock<double>> ketblock, double* target) const {
        const int nbras = bras->ij();

        if (bras->det()->allowed(ketblock->stringsb(), ketblock->stringsa())) {
          for (int jbra = 0; jbra < nbras; ++jbra, ++target) {
            std::shared_ptr<const RASBlock<double>> brablock = bras->data(jbra)->block(ketblock->stringsb(), ketblock->stringsa());
            if (brablock)
              *target += blas::dot_product(brablock->data(), brablock->size(), ketblock->data());
          }
        }
      }

      std::shared_ptr<const RASString> stringspace(const int a, const int b, const int c, const int d, const int e, const int f) final {
        auto iter = stringspaces_.find(std::make_tuple(a,b,c,d,e,f));
        if (iter != stringspaces_.end()) {
          return iter->second;
        }
        else {
          stringspaces_.emplace(std::make_tuple(a,b,c,d,e,f), std::make_shared<RASString>(a,b,c,d,e,f));
          return stringspaces_[std::make_tuple(a,b,c,d,e,f)];
        }
      }
};

}

#endif
