//
// BAGEL - Parallel electron correlation program.
// Filename: storage.h
// Copyright (C) 2012 Toru Shiozaki
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

//
// this is a base class of storage object
// ... It can be distributed memory, distributed disk, etc.
// ... All of them should be derived from here with the same interface.

#ifndef __SRC_SMITH_STORAGE_H
#define __SRC_SMITH_STORAGE_H

#include <stddef.h>
#include <map>
#include <memory>
#include <tuple>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <cstdio>

namespace bagel {
namespace SMITH {

template<class BlockType>
class Storage_base {
  protected:
    // this relates hash keys, block number, and block lengths (in this order).
    std::map<size_t, std::shared_ptr<BlockType>> hashtable_;

  public:
    // size contains hashkey and length (in this order)
    Storage_base(const std::map<size_t, size_t>& size, bool init) {
      for (auto& i : size)
        hashtable_.emplace(i.first, std::make_shared<BlockType>(i.second, init));
    }

    // functions that return protected members
    size_t blocksize(const size_t hash) const {
      auto a = hashtable_.find(hash);
      return a != hashtable_.end() ? a->second->size() : 0lu;
    }
    size_t blocksize_alloc(const size_t hash) const {
      auto a = hashtable_.find(hash);
      return a != hashtable_.end() ? a->second->size_alloc() : 0lu;
    }

    size_t size() const {
      return std::accumulate(hashtable_.begin(), hashtable_.end(), 0lu, [](size_t sum, const std::pair<size_t, std::shared_ptr<BlockType>>& o) { return sum+o.second->size(); });
    }

    size_t size_alloc() const {
      return std::accumulate(hashtable_.begin(), hashtable_.end(), 0lu, [](size_t sum, const std::pair<size_t, std::shared_ptr<BlockType>>& o) { return sum+o.second->size_alloc(); });
    }

    // get, move, put, and add a block from the storage and returns unique_ptr<double[]>, which is local
    virtual std::unique_ptr<double[]> get_block(const size_t& key) const = 0;
    virtual std::unique_ptr<double[]> move_block(const size_t& key) = 0;
    virtual void put_block(const size_t& key, std::unique_ptr<double[]>& dat) = 0;
    virtual void add_block(const size_t& key, const std::unique_ptr<double[]>& dat) = 0;

    virtual void zero() = 0;
    virtual void scale(const double a) = 0;

};


class StorageBlock {
  protected:
    std::unique_ptr<double[]> data_;
    size_t size_;
    bool initialized_;

    double* data() { return data_.get(); }
    const double* data() const { return data_.get(); }
  public:
    StorageBlock(const size_t size, const bool init);

    void zero();

    size_t size() const { return size_; }
    size_t size_alloc() const { return initialized_ ? size_ : 0lu; }

    StorageBlock& operator=(const StorageBlock& o);

    void put_block(std::unique_ptr<double[]>&& o);
    void add_block(const std::unique_ptr<double[]>& o);

    std::unique_ptr<double[]> get_block() const;
    std::unique_ptr<double[]> move_block();

    double dot_product(const StorageBlock& o) const;
    void ax_plus_y(const double a, const StorageBlock& o);
    void scale(const double a);
};


class Storage_Incore : public Storage_base<StorageBlock> {
  public:
    Storage_Incore(const std::map<size_t, size_t>& size, bool init);

    std::unique_ptr<double[]> get_block(const size_t& key) const;
    std::unique_ptr<double[]> move_block(const size_t& key);
    void put_block(const size_t& key, std::unique_ptr<double[]>& dat);
    void add_block(const size_t& key, const std::unique_ptr<double[]>& dat);

    void zero();
    void scale(const double a);

    Storage_Incore& operator=(const Storage_Incore& o);
    void ax_plus_y(const double a, const Storage_Incore& o);
    void ax_plus_y(const double a, const std::shared_ptr<Storage_Incore> o) { ax_plus_y(a, *o); };
    double dot_product(const Storage_Incore& o) const;

};

using Storage = Storage_Incore;

}
}

#endif
