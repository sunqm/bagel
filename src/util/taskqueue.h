//
// BAGEL - Parallel electron correlation program.
// Filename: taskqueue.h
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


#ifndef __SRC_UTIL_TASKQUEUE_H
#define __SRC_UTIL_TASKQUEUE_H

#include <stddef.h>
#include <list>
#include <atomic>
#include <memory>
#include <stdexcept>
#include <utility>
#include <thread>
#include <algorithm>

#include <vector>
#include <bagel_config.h>
#ifdef HAVE_MKL_H
  #include "mkl_service.h"
#endif
#include <src/util/parallel/resources.h>

namespace bagel {

template<typename T>
class TaskQueue {

  private:
    template <class U>
    struct has_compute {
      protected:
        template<class V> static auto __compute(V* p) -> decltype(p->compute(), std::true_type());
        template<class  > static std::false_type __compute(...);
      public:
        static constexpr const bool value = std::is_same<std::true_type, decltype(__compute<U>(0))>::value;
    };
    template<typename U, bool>
    struct call       { static void compute(U& task) { assert(false); } };
    template<typename U>
    struct call<U, true> { static void compute(U& task) { task.compute(); } };
    template<typename U>
    struct call<U, false>{ static void compute(U& task) { task(); } };
    template<typename U> void call_compute(U& task)                  { call<U, has_compute<U>::value>::compute(task); }
    template<typename U> void call_compute(std::shared_ptr<U>& task) { call<U, has_compute<U>::value>::compute(*task); }

  protected:
    std::vector<T> task_;
    std::list<std::atomic_flag> flag_;
    static const int chunck_ = 12;

  public:
    TaskQueue(size_t expected = 0) { task_.reserve(expected); }
    TaskQueue(std::vector<T>&& t) : task_(std::move(t)) { }

    template<typename ...args>
    void emplace_back(args&&... a) { task_.emplace_back(std::forward<args>(a)...); }

    void compute(const int num_threads = resources__->max_num_threads()) {
      if (task_.empty()) return;
#ifdef HAVE_MKL_H
      const int mkl_num = mkl_get_max_threads();
      mkl_set_num_threads(1);
#endif
#ifndef _OPENMP
      flag_.resize((task_.size()-1)/chunck_+1);
      std::for_each(flag_.begin(), flag_.end(), [](std::atomic_flag& i){ i.clear(); });
      std::list<std::thread> threads;
      for (int i = 0; i != num_threads; ++i)
        threads.emplace_back(&TaskQueue<T>::compute_one_thread, this);
      std::for_each(threads.begin(), threads.end(), [](std::thread& i){ i.join(); });
#else
      const size_t n = task_.size();
      #pragma omp parallel for schedule(dynamic,chunck_)
      for (size_t i = 0; i < n; ++i)
        call_compute(task_[i]);
#endif
#ifdef HAVE_MKL_H
      mkl_set_num_threads(mkl_num);
#endif
    }

    void compute_one_thread() {
      int j = 0;
      for (auto i = flag_.begin(); i != flag_.end(); ++i, j += chunck_)
        if (!i->test_and_set()) {
          call_compute(task_[j]);
          for (int k = 1; k < chunck_; ++k)
            if (j+k < task_.size()) call_compute(task_[j+k]);
        }
    }
};

}

#endif
