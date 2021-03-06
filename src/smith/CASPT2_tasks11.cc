//
// BAGEL - Parallel electron correlation program.
// Filename: CASPT2_tasks11.cc
// Copyright (C) 2014 Shiozaki group
//
// Author: Shiozaki group <shiozaki@northwestern.edu>
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

#include <bagel_config.h>
#ifdef COMPILE_SMITH

#include <src/smith/CASPT2_tasks11.h>

using namespace std;
using namespace bagel;
using namespace bagel::SMITH;
using namespace bagel::SMITH::CASPT2;

void Task500::Task_local::compute() {
  const Index a1 = b(0);
  const Index x3 = b(1);
  // tensor label: I628
  std::unique_ptr<double[]> odata = out()->move_block(a1, x3);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a1, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a1, x3), 0.0);
  for (auto& x0 : *range_[1]) {
    for (auto& x2 : *range_[1]) {
      for (auto& x1 : *range_[1]) {
        // tensor label: Gamma60
        std::unique_ptr<double[]> i0data = in(0)->get_block(x3, x0, x2, x1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, x0, x2, x1)]);
        sort_indices<1,2,3,0,0,1,1,1>(i0data, i0data_sorted, x3.size(), x0.size(), x2.size(), x1.size());
        // tensor label: I629
        std::unique_ptr<double[]> i1data = in(1)->get_block(x2, x1, a1, x0);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x2, x1, a1, x0)]);
        sort_indices<3,0,1,2,0,1,1,1>(i1data, i1data_sorted, x2.size(), x1.size(), a1.size(), x0.size());
        dgemm_("T", "N", x3.size(), a1.size(), x2.size()*x1.size()*x0.size(),
               1.0, i0data_sorted, x2.size()*x1.size()*x0.size(), i1data_sorted, x2.size()*x1.size()*x0.size(),
               1.0, odata_sorted, x3.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x3.size(), a1.size());
  out()->put_block(odata, a1, x3);
}

void Task501::Task_local::compute() {
  const Index x2 = b(0);
  const Index x1 = b(1);
  const Index a1 = b(2);
  const Index x0 = b(3);
  // tensor label: I629
  std::unique_ptr<double[]> odata = out()->move_block(x2, x1, a1, x0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, x1, x2);
    sort_indices<3,2,1,0,1,1,2,1>(i0data, odata, x0.size(), a1.size(), x1.size(), x2.size());
  }
  out()->put_block(odata, x2, x1, a1, x0);
}

void Task502::Task_local::compute() {
  const Index x3 = b(0);
  const Index a2 = b(1);
  // tensor label: r
  std::unique_ptr<double[]> odata = out()->move_block(x3, a2);
  {
    // tensor label: I471
    std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a2);
    sort_indices<0,1,1,1,1,1>(i0data, odata, x3.size(), a2.size());
  }
  out()->put_block(odata, x3, a2);
}

void Task503::Task_local::compute() {
  const Index x3 = b(0);
  const Index a2 = b(1);
  // tensor label: I471
  std::unique_ptr<double[]> odata = out()->move_block(x3, a2);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, a2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, a2), 0.0);
  for (auto& x5 : *range_[1]) {
    for (auto& c1 : *range_[0]) {
      for (auto& x4 : *range_[1]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(x5, a2, c1, x4);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, a2, c1, x4)]);
        sort_indices<0,2,3,1,0,1,1,1>(i0data, i0data_sorted, x5.size(), a2.size(), c1.size(), x4.size());
        // tensor label: I472
        std::unique_ptr<double[]> i1data = in(1)->get_block(c1, x5, x3, x4);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, x5, x3, x4)]);
        sort_indices<1,0,3,2,0,1,1,1>(i1data, i1data_sorted, c1.size(), x5.size(), x3.size(), x4.size());
        dgemm_("T", "N", a2.size(), x3.size(), c1.size()*x5.size()*x4.size(),
               1.0, i0data_sorted, c1.size()*x5.size()*x4.size(), i1data_sorted, c1.size()*x5.size()*x4.size(),
               1.0, odata_sorted, a2.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, a2.size(), x3.size());
  out()->put_block(odata, x3, a2);
}

void Task504::Task_local::compute() {
  const Index c1 = b(0);
  const Index x5 = b(1);
  const Index x3 = b(2);
  const Index x4 = b(3);
  // tensor label: I472
  std::unique_ptr<double[]> odata = out()->move_block(c1, x5, x3, x4);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c1, x5, x3, x4)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c1, x5, x3, x4), 0.0);
  for (auto& x2 : *range_[1]) {
    for (auto& x1 : *range_[1]) {
      for (auto& x0 : *range_[1]) {
        // tensor label: Gamma9
        std::unique_ptr<double[]> i0data = in(0)->get_block(x5, x3, x2, x4, x1, x0);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, x3, x2, x4, x1, x0)]);
        sort_indices<2,4,5,0,1,3,0,1,1,1>(i0data, i0data_sorted, x5.size(), x3.size(), x2.size(), x4.size(), x1.size(), x0.size());
        // tensor label: I473
        std::unique_ptr<double[]> i1data = in(1)->get_block(x2, c1, x1, x0);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x2, c1, x1, x0)]);
        sort_indices<0,2,3,1,0,1,1,1>(i1data, i1data_sorted, x2.size(), c1.size(), x1.size(), x0.size());
        dgemm_("T", "N", x5.size()*x3.size()*x4.size(), c1.size(), x2.size()*x1.size()*x0.size(),
               1.0, i0data_sorted, x2.size()*x1.size()*x0.size(), i1data_sorted, x2.size()*x1.size()*x0.size(),
               1.0, odata_sorted, x5.size()*x3.size()*x4.size());
      }
    }
  }
  sort_indices<3,0,1,2,1,1,1,1>(odata_sorted, odata, x5.size(), x3.size(), x4.size(), c1.size());
  out()->put_block(odata, c1, x5, x3, x4);
}

void Task505::Task_local::compute() {
  const Index x2 = b(0);
  const Index c1 = b(1);
  const Index x1 = b(2);
  const Index x0 = b(3);
  // tensor label: I473
  std::unique_ptr<double[]> odata = out()->move_block(x2, c1, x1, x0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, x1, c1, x2);
    sort_indices<3,2,1,0,1,1,-1,1>(i0data, odata, x0.size(), x1.size(), c1.size(), x2.size());
  }
  out()->put_block(odata, x2, c1, x1, x0);
}

void Task506::Task_local::compute() {
  const Index x3 = b(0);
  const Index a2 = b(1);
  // tensor label: I471
  std::unique_ptr<double[]> odata = out()->move_block(x3, a2);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, a2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, a2), 0.0);
  for (auto& c1 : *range_[0]) {
    for (auto& x5 : *range_[1]) {
      for (auto& x4 : *range_[1]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, x5, x4);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, x5, x4)]);
        sort_indices<0,2,3,1,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), x5.size(), x4.size());
        // tensor label: I475
        std::unique_ptr<double[]> i1data = in(1)->get_block(c1, x5, x4, x3);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, x5, x4, x3)]);
        sort_indices<0,1,2,3,0,1,1,1>(i1data, i1data_sorted, c1.size(), x5.size(), x4.size(), x3.size());
        dgemm_("T", "N", a2.size(), x3.size(), c1.size()*x5.size()*x4.size(),
               1.0, i0data_sorted, c1.size()*x5.size()*x4.size(), i1data_sorted, c1.size()*x5.size()*x4.size(),
               1.0, odata_sorted, a2.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, a2.size(), x3.size());
  out()->put_block(odata, x3, a2);
}

void Task507::Task_local::compute() {
  const Index c1 = b(0);
  const Index x5 = b(1);
  const Index x4 = b(2);
  const Index x3 = b(3);
  // tensor label: I475
  std::unique_ptr<double[]> odata = out()->move_block(c1, x5, x4, x3);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c1, x5, x4, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c1, x5, x4, x3), 0.0);
  for (auto& x2 : *range_[1]) {
    for (auto& x1 : *range_[1]) {
      for (auto& x0 : *range_[1]) {
        // tensor label: Gamma6
        std::unique_ptr<double[]> i0data = in(0)->get_block(x5, x4, x2, x3, x1, x0);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, x4, x2, x3, x1, x0)]);
        sort_indices<2,4,5,0,1,3,0,1,1,1>(i0data, i0data_sorted, x5.size(), x4.size(), x2.size(), x3.size(), x1.size(), x0.size());
        // tensor label: I476
        std::unique_ptr<double[]> i1data = in(1)->get_block(x2, c1, x1, x0);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x2, c1, x1, x0)]);
        sort_indices<0,2,3,1,0,1,1,1>(i1data, i1data_sorted, x2.size(), c1.size(), x1.size(), x0.size());
        dgemm_("T", "N", x5.size()*x4.size()*x3.size(), c1.size(), x2.size()*x1.size()*x0.size(),
               1.0, i0data_sorted, x2.size()*x1.size()*x0.size(), i1data_sorted, x2.size()*x1.size()*x0.size(),
               1.0, odata_sorted, x5.size()*x4.size()*x3.size());
      }
    }
  }
  sort_indices<3,0,1,2,1,1,1,1>(odata_sorted, odata, x5.size(), x4.size(), x3.size(), c1.size());
  out()->put_block(odata, c1, x5, x4, x3);
}

void Task508::Task_local::compute() {
  const Index x2 = b(0);
  const Index c1 = b(1);
  const Index x1 = b(2);
  const Index x0 = b(3);
  // tensor label: I476
  std::unique_ptr<double[]> odata = out()->move_block(x2, c1, x1, x0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, x1, c1, x2);
    sort_indices<3,2,1,0,1,1,1,1>(i0data, odata, x0.size(), x1.size(), c1.size(), x2.size());
  }
  out()->put_block(odata, x2, c1, x1, x0);
}

void Task509::Task_local::compute() {
  const Index x3 = b(0);
  const Index a2 = b(1);
  // tensor label: I471
  std::unique_ptr<double[]> odata = out()->move_block(x3, a2);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, a2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, a2), 0.0);
  for (auto& x5 : *range_[1]) {
    for (auto& a1 : *range_[2]) {
      for (auto& x4 : *range_[1]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(x5, a1, x4, a2);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, a1, x4, a2)]);
        sort_indices<0,1,2,3,0,1,1,1>(i0data, i0data_sorted, x5.size(), a1.size(), x4.size(), a2.size());
        // tensor label: I631
        std::unique_ptr<double[]> i1data = in(1)->get_block(a1, x5, x4, x3);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a1, x5, x4, x3)]);
        sort_indices<1,0,2,3,0,1,1,1>(i1data, i1data_sorted, a1.size(), x5.size(), x4.size(), x3.size());
        dgemm_("T", "N", a2.size(), x3.size(), a1.size()*x5.size()*x4.size(),
               1.0, i0data_sorted, a1.size()*x5.size()*x4.size(), i1data_sorted, a1.size()*x5.size()*x4.size(),
               1.0, odata_sorted, a2.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, a2.size(), x3.size());
  out()->put_block(odata, x3, a2);
}

void Task510::Task_local::compute() {
  const Index a1 = b(0);
  const Index x5 = b(1);
  const Index x4 = b(2);
  const Index x3 = b(3);
  // tensor label: I631
  std::unique_ptr<double[]> odata = out()->move_block(a1, x5, x4, x3);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a1, x5, x4, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a1, x5, x4, x3), 0.0);
  for (auto& x0 : *range_[1]) {
    for (auto& x2 : *range_[1]) {
      for (auto& x1 : *range_[1]) {
        // tensor label: Gamma59
        std::unique_ptr<double[]> i0data = in(0)->get_block(x5, x0, x4, x3, x2, x1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, x0, x4, x3, x2, x1)]);
        sort_indices<1,4,5,0,2,3,0,1,1,1>(i0data, i0data_sorted, x5.size(), x0.size(), x4.size(), x3.size(), x2.size(), x1.size());
        // tensor label: I632
        std::unique_ptr<double[]> i1data = in(1)->get_block(x2, x1, a1, x0);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x2, x1, a1, x0)]);
        sort_indices<3,0,1,2,0,1,1,1>(i1data, i1data_sorted, x2.size(), x1.size(), a1.size(), x0.size());
        dgemm_("T", "N", x5.size()*x4.size()*x3.size(), a1.size(), x2.size()*x1.size()*x0.size(),
               1.0, i0data_sorted, x2.size()*x1.size()*x0.size(), i1data_sorted, x2.size()*x1.size()*x0.size(),
               1.0, odata_sorted, x5.size()*x4.size()*x3.size());
      }
    }
  }
  sort_indices<3,0,1,2,1,1,1,1>(odata_sorted, odata, x5.size(), x4.size(), x3.size(), a1.size());
  out()->put_block(odata, a1, x5, x4, x3);
}

void Task511::Task_local::compute() {
  const Index x2 = b(0);
  const Index x1 = b(1);
  const Index a1 = b(2);
  const Index x0 = b(3);
  // tensor label: I632
  std::unique_ptr<double[]> odata = out()->move_block(x2, x1, a1, x0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, x1, x2);
    sort_indices<3,2,1,0,1,1,2,1>(i0data, odata, x0.size(), a1.size(), x1.size(), x2.size());
  }
  out()->put_block(odata, x2, x1, a1, x0);
}

void Task512::Task_local::compute() {
  const Index a2 = b(0);
  const Index x1 = b(1);
  // tensor label: r
  std::unique_ptr<double[]> odata = out()->move_block(a2, x1);
  {
    // tensor label: I477
    std::unique_ptr<double[]> i0data = in(0)->get_block(x1, a2);
    sort_indices<1,0,1,1,1,1>(i0data, odata, x1.size(), a2.size());
  }
  out()->put_block(odata, a2, x1);
}

void Task513::Task_local::compute() {
  const Index x1 = b(0);
  const Index a2 = b(1);
  // tensor label: I477
  std::unique_ptr<double[]> odata = out()->move_block(x1, a2);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x1, a2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x1, a2), 0.0);
  for (auto& x0 : *range_[1]) {
    for (auto& c3 : *range_[0]) {
      for (auto& c1 : *range_[0]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x0);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c3, x0)]);
        sort_indices<3,2,0,1,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c3.size(), x0.size());
        // tensor label: I478
        std::unique_ptr<double[]> i1data = in(1)->get_block(c1, c3, x1, x0);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, c3, x1, x0)]);
        sort_indices<3,1,0,2,0,1,1,1>(i1data, i1data_sorted, c1.size(), c3.size(), x1.size(), x0.size());
        dgemm_("T", "N", a2.size(), x1.size(), c1.size()*c3.size()*x0.size(),
               1.0, i0data_sorted, c1.size()*c3.size()*x0.size(), i1data_sorted, c1.size()*c3.size()*x0.size(),
               1.0, odata_sorted, a2.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, a2.size(), x1.size());
  out()->put_block(odata, x1, a2);
}

void Task514::Task_local::compute() {
  const Index c1 = b(0);
  const Index c3 = b(1);
  const Index x1 = b(2);
  const Index x0 = b(3);
  // tensor label: I478
  std::unique_ptr<double[]> odata = out()->move_block(c1, c3, x1, x0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c1, c3, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c1, c3, x1, x0), 0.0);
  for (auto& x3 : *range_[1]) {
    for (auto& x2 : *range_[1]) {
      // tensor label: Gamma3
      std::unique_ptr<double[]> i0data = in(0)->get_block(x1, x3, x0, x2);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x1, x3, x0, x2)]);
      sort_indices<1,3,0,2,0,1,1,1>(i0data, i0data_sorted, x1.size(), x3.size(), x0.size(), x2.size());
      // tensor label: I479
      std::unique_ptr<double[]> i1data = in(1)->get_block(c1, x3, c3, x2);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, x3, c3, x2)]);
      sort_indices<1,3,0,2,0,1,1,1>(i1data, i1data_sorted, c1.size(), x3.size(), c3.size(), x2.size());
      dgemm_("T", "N", x1.size()*x0.size(), c1.size()*c3.size(), x3.size()*x2.size(),
             1.0, i0data_sorted, x3.size()*x2.size(), i1data_sorted, x3.size()*x2.size(),
             1.0, odata_sorted, x1.size()*x0.size());
    }
  }
  sort_indices<2,3,0,1,1,1,1,1>(odata_sorted, odata, x1.size(), x0.size(), c1.size(), c3.size());
  out()->put_block(odata, c1, c3, x1, x0);
}

void Task515::Task_local::compute() {
  const Index c1 = b(0);
  const Index x3 = b(1);
  const Index c3 = b(2);
  const Index x2 = b(3);
  // tensor label: I479
  std::unique_ptr<double[]> odata = out()->move_block(c1, x3, c3, x2);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(c1, x3, c3, x2);
    sort_indices<0,1,2,3,1,1,-2,1>(i0data, odata, c1.size(), x3.size(), c3.size(), x2.size());
  }
  out()->put_block(odata, c1, x3, c3, x2);
}

void Task516::Task_local::compute() {
  const Index a2 = b(0);
  const Index c1 = b(1);
  // tensor label: r
  std::unique_ptr<double[]> odata = out()->move_block(a2, c1);
  {
    // tensor label: I480
    std::unique_ptr<double[]> i0data = in(0)->get_block(a2, c1);
    sort_indices<0,1,1,1,1,1>(i0data, odata, a2.size(), c1.size());
  }
  out()->put_block(odata, a2, c1);
}

void Task517::Task_local::compute() {
  const Index a2 = b(0);
  const Index c1 = b(1);
  // tensor label: I480
  std::unique_ptr<double[]> odata = out()->move_block(a2, c1);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a2, c1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a2, c1), 0.0);
  for (auto& x0 : *range_[1]) {
    for (auto& c3 : *range_[0]) {
      // tensor label: t2
      std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x0);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c3, x0)]);
      sort_indices<3,2,0,1,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c3.size(), x0.size());
      // tensor label: I481
      std::unique_ptr<double[]> i1data = in(1)->get_block(c3, x0);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c3, x0)]);
      sort_indices<1,0,0,1,1,1>(i1data, i1data_sorted, c3.size(), x0.size());
      dgemm_("T", "N", a2.size()*c1.size(), 1, c3.size()*x0.size(),
             1.0, i0data_sorted, c3.size()*x0.size(), i1data_sorted, c3.size()*x0.size(),
             1.0, odata_sorted, a2.size()*c1.size());
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, c1.size(), a2.size());
  out()->put_block(odata, a2, c1);
}

void Task518::Task_local::compute() {
  const Index c3 = b(0);
  const Index x0 = b(1);
  // tensor label: I481
  std::unique_ptr<double[]> odata = out()->move_block(c3, x0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, x0), 0.0);
  for (auto& x3 : *range_[1]) {
    for (auto& x2 : *range_[1]) {
      for (auto& x1 : *range_[1]) {
        // tensor label: Gamma12
        std::unique_ptr<double[]> i0data = in(0)->get_block(x3, x2, x0, x1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, x2, x0, x1)]);
        sort_indices<0,1,3,2,0,1,1,1>(i0data, i0data_sorted, x3.size(), x2.size(), x0.size(), x1.size());
        // tensor label: I482
        std::unique_ptr<double[]> i1data = in(1)->get_block(x3, x2, c3, x1);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x3, x2, c3, x1)]);
        sort_indices<0,1,3,2,0,1,1,1>(i1data, i1data_sorted, x3.size(), x2.size(), c3.size(), x1.size());
        dgemm_("T", "N", x0.size(), c3.size(), x3.size()*x2.size()*x1.size(),
               1.0, i0data_sorted, x3.size()*x2.size()*x1.size(), i1data_sorted, x3.size()*x2.size()*x1.size(),
               1.0, odata_sorted, x0.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x0.size(), c3.size());
  out()->put_block(odata, c3, x0);
}

void Task519::Task_local::compute() {
  const Index x3 = b(0);
  const Index x2 = b(1);
  const Index c3 = b(2);
  const Index x1 = b(3);
  // tensor label: I482
  std::unique_ptr<double[]> odata = out()->move_block(x3, x2, c3, x1);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x3, x2, c3, x1);
    sort_indices<0,1,2,3,1,1,2,1>(i0data, odata, x3.size(), x2.size(), c3.size(), x1.size());
  }
  out()->put_block(odata, x3, x2, c3, x1);
}

void Task520::Task_local::compute() {
  const Index a2 = b(0);
  const Index c3 = b(1);
  // tensor label: r
  std::unique_ptr<double[]> odata = out()->move_block(a2, c3);
  {
    // tensor label: I483
    std::unique_ptr<double[]> i0data = in(0)->get_block(c3, a2);
    sort_indices<1,0,1,1,1,1>(i0data, odata, c3.size(), a2.size());
  }
  out()->put_block(odata, a2, c3);
}

void Task521::Task_local::compute() {
  const Index c3 = b(0);
  const Index a2 = b(1);
  // tensor label: I483
  std::unique_ptr<double[]> odata = out()->move_block(c3, a2);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, a2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, a2), 0.0);
  for (auto& x0 : *range_[1]) {
    for (auto& c1 : *range_[0]) {
      // tensor label: t2
      std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x0);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c3, x0)]);
      sort_indices<3,0,1,2,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c3.size(), x0.size());
      // tensor label: I484
      std::unique_ptr<double[]> i1data = in(1)->get_block(c1, x0);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, x0)]);
      sort_indices<1,0,0,1,1,1>(i1data, i1data_sorted, c1.size(), x0.size());
      dgemm_("T", "N", c3.size()*a2.size(), 1, c1.size()*x0.size(),
             1.0, i0data_sorted, c1.size()*x0.size(), i1data_sorted, c1.size()*x0.size(),
             1.0, odata_sorted, c3.size()*a2.size());
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, a2.size(), c3.size());
  out()->put_block(odata, c3, a2);
}

void Task522::Task_local::compute() {
  const Index c1 = b(0);
  const Index x0 = b(1);
  // tensor label: I484
  std::unique_ptr<double[]> odata = out()->move_block(c1, x0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c1, x0), 0.0);
  for (auto& x3 : *range_[1]) {
    for (auto& x2 : *range_[1]) {
      for (auto& x1 : *range_[1]) {
        // tensor label: Gamma12
        std::unique_ptr<double[]> i0data = in(0)->get_block(x3, x2, x0, x1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, x2, x0, x1)]);
        sort_indices<0,1,3,2,0,1,1,1>(i0data, i0data_sorted, x3.size(), x2.size(), x0.size(), x1.size());
        // tensor label: I485
        std::unique_ptr<double[]> i1data = in(1)->get_block(x3, x2, c1, x1);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x3, x2, c1, x1)]);
        sort_indices<0,1,3,2,0,1,1,1>(i1data, i1data_sorted, x3.size(), x2.size(), c1.size(), x1.size());
        dgemm_("T", "N", x0.size(), c1.size(), x3.size()*x2.size()*x1.size(),
               1.0, i0data_sorted, x3.size()*x2.size()*x1.size(), i1data_sorted, x3.size()*x2.size()*x1.size(),
               1.0, odata_sorted, x0.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x0.size(), c1.size());
  out()->put_block(odata, c1, x0);
}

void Task523::Task_local::compute() {
  const Index x3 = b(0);
  const Index x2 = b(1);
  const Index c1 = b(2);
  const Index x1 = b(3);
  // tensor label: I485
  std::unique_ptr<double[]> odata = out()->move_block(x3, x2, c1, x1);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x3, x2, c1, x1);
    sort_indices<0,1,2,3,1,1,-1,1>(i0data, odata, x3.size(), x2.size(), c1.size(), x1.size());
  }
  out()->put_block(odata, x3, x2, c1, x1);
}

void Task524::Task_local::compute() {
  const Index c3 = b(0);
  const Index a2 = b(1);
  // tensor label: I483
  std::unique_ptr<double[]> odata = out()->move_block(c3, a2);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, a2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, a2), 0.0);
  for (auto& a4 : *range_[2]) {
    for (auto& c1 : *range_[0]) {
      // tensor label: t2
      std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, a4);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c3, a4)]);
      sort_indices<3,0,1,2,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c3.size(), a4.size());
      // tensor label: I640
      std::unique_ptr<double[]> i1data = in(1)->get_block(a4, c1);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a4, c1)]);
      sort_indices<0,1,0,1,1,1>(i1data, i1data_sorted, a4.size(), c1.size());
      dgemm_("T", "N", c3.size()*a2.size(), 1, a4.size()*c1.size(),
             1.0, i0data_sorted, a4.size()*c1.size(), i1data_sorted, a4.size()*c1.size(),
             1.0, odata_sorted, c3.size()*a2.size());
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, a2.size(), c3.size());
  out()->put_block(odata, c3, a2);
}

void Task525::Task_local::compute() {
  const Index a4 = b(0);
  const Index c1 = b(1);
  // tensor label: I640
  std::unique_ptr<double[]> odata = out()->move_block(a4, c1);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a4, c1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a4, c1), 0.0);
  for (auto& x1 : *range_[1]) {
    for (auto& x0 : *range_[1]) {
      // tensor label: Gamma38
      std::unique_ptr<double[]> i0data = in(0)->get_block(x1, x0);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x1, x0)]);
      sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, x1.size(), x0.size());
      // tensor label: I641
      std::unique_ptr<double[]> i1data = in(1)->get_block(x1, a4, c1, x0);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x1, a4, c1, x0)]);
      sort_indices<0,3,1,2,0,1,1,1>(i1data, i1data_sorted, x1.size(), a4.size(), c1.size(), x0.size());
      dgemm_("T", "N", 1, a4.size()*c1.size(), x1.size()*x0.size(),
             1.0, i0data_sorted, x1.size()*x0.size(), i1data_sorted, x1.size()*x0.size(),
             1.0, odata_sorted, 1);
    }
  }
  sort_indices<0,1,1,1,1,1>(odata_sorted, odata, a4.size(), c1.size());
  out()->put_block(odata, a4, c1);
}

void Task526::Task_local::compute() {
  const Index x1 = b(0);
  const Index a4 = b(1);
  const Index c1 = b(2);
  const Index x0 = b(3);
  // tensor label: I641
  std::unique_ptr<double[]> odata = out()->move_block(x1, a4, c1, x0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x1, a4, c1, x0);
    sort_indices<0,1,2,3,1,1,2,1>(i0data, odata, x1.size(), a4.size(), c1.size(), x0.size());
  }
  {
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(0)->get_block(c1, a4, x1, x0);
    sort_indices<2,1,0,3,1,1,-4,1>(i1data, odata, c1.size(), a4.size(), x1.size(), x0.size());
  }
  out()->put_block(odata, x1, a4, c1, x0);
}

void Task527::Task_local::compute() {
  const Index x1 = b(0);
  const Index x2 = b(1);
  // tensor label: r
  std::unique_ptr<double[]> odata = out()->move_block(x1, x2);
  {
    // tensor label: I486
    std::unique_ptr<double[]> i0data = in(0)->get_block(x2, x1);
    sort_indices<1,0,1,1,1,1>(i0data, odata, x2.size(), x1.size());
  }
  out()->put_block(odata, x1, x2);
}

void Task528::Task_local::compute() {
  const Index x2 = b(0);
  const Index x1 = b(1);
  // tensor label: I486
  std::unique_ptr<double[]> odata = out()->move_block(x2, x1);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x2, x1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x2, x1), 0.0);
  for (auto& x0 : *range_[1]) {
    for (auto& x3 : *range_[1]) {
      // tensor label: Gamma174
      std::unique_ptr<double[]> i0data = in(0)->get_block(x0, x3, x2, x1);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, x3, x2, x1)]);
      sort_indices<0,1,2,3,0,1,1,1>(i0data, i0data_sorted, x0.size(), x3.size(), x2.size(), x1.size());
      // tensor label: I487
      std::unique_ptr<double[]> i1data = in(1)->get_block(x0, x3);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, x3)]);
      sort_indices<0,1,0,1,1,1>(i1data, i1data_sorted, x0.size(), x3.size());
      dgemm_("T", "N", x2.size()*x1.size(), 1, x0.size()*x3.size(),
             1.0, i0data_sorted, x0.size()*x3.size(), i1data_sorted, x0.size()*x3.size(),
             1.0, odata_sorted, x2.size()*x1.size());
    }
  }
  sort_indices<0,1,1,1,1,1>(odata_sorted, odata, x2.size(), x1.size());
  out()->put_block(odata, x2, x1);
}

void Task529::Task_local::compute() {
  const Index x0 = b(0);
  const Index x3 = b(1);
  // tensor label: I487
  std::unique_ptr<double[]> odata = out()->move_block(x0, x3);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x0, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x0, x3), 0.0);
  for (auto& c3 : *range_[0]) {
    for (auto& a2 : *range_[2]) {
      for (auto& c1 : *range_[0]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c3, a2, c1, x3);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c3, a2, c1, x3)]);
        sort_indices<0,1,2,3,0,1,1,1>(i0data, i0data_sorted, c3.size(), a2.size(), c1.size(), x3.size());
        // tensor label: I488
        std::unique_ptr<double[]> i1data = in(1)->get_block(x0, c3, a2, c1);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, c3, a2, c1)]);
        sort_indices<1,2,3,0,0,1,1,1>(i1data, i1data_sorted, x0.size(), c3.size(), a2.size(), c1.size());
        dgemm_("T", "N", x3.size(), x0.size(), c3.size()*a2.size()*c1.size(),
               1.0, i0data_sorted, c3.size()*a2.size()*c1.size(), i1data_sorted, c3.size()*a2.size()*c1.size(),
               1.0, odata_sorted, x3.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x3.size(), x0.size());
  out()->put_block(odata, x0, x3);
}

void Task530::Task_local::compute() {
  const Index x0 = b(0);
  const Index c3 = b(1);
  const Index a2 = b(2);
  const Index c1 = b(3);
  // tensor label: I488
  std::unique_ptr<double[]> odata = out()->move_block(x0, c3, a2, c1);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x0);
    sort_indices<3,2,1,0,1,1,-1,1>(i0data, odata, c1.size(), a2.size(), c3.size(), x0.size());
  }
  out()->put_block(odata, x0, c3, a2, c1);
}

void Task531::Task_local::compute() {
  const Index x0 = b(0);
  const Index x3 = b(1);
  // tensor label: I487
  std::unique_ptr<double[]> odata = out()->move_block(x0, x3);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x0, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x0, x3), 0.0);
  for (auto& c1 : *range_[0]) {
    for (auto& a2 : *range_[2]) {
      for (auto& c3 : *range_[0]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x3);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c3, x3)]);
        sort_indices<0,1,2,3,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c3.size(), x3.size());
        // tensor label: I491
        std::unique_ptr<double[]> i1data = in(1)->get_block(x0, c3, a2, c1);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, c3, a2, c1)]);
        sort_indices<3,2,1,0,0,1,1,1>(i1data, i1data_sorted, x0.size(), c3.size(), a2.size(), c1.size());
        dgemm_("T", "N", x3.size(), x0.size(), c3.size()*a2.size()*c1.size(),
               1.0, i0data_sorted, c3.size()*a2.size()*c1.size(), i1data_sorted, c3.size()*a2.size()*c1.size(),
               1.0, odata_sorted, x3.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x3.size(), x0.size());
  out()->put_block(odata, x0, x3);
}

void Task532::Task_local::compute() {
  const Index x0 = b(0);
  const Index c3 = b(1);
  const Index a2 = b(2);
  const Index c1 = b(3);
  // tensor label: I491
  std::unique_ptr<double[]> odata = out()->move_block(x0, c3, a2, c1);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x0);
    sort_indices<3,2,1,0,1,1,2,1>(i0data, odata, c1.size(), a2.size(), c3.size(), x0.size());
  }
  out()->put_block(odata, x0, c3, a2, c1);
}

void Task533::Task_local::compute() {
  const Index x2 = b(0);
  const Index x1 = b(1);
  // tensor label: I486
  std::unique_ptr<double[]> odata = out()->move_block(x2, x1);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x2, x1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x2, x1), 0.0);
  for (auto& x3 : *range_[1]) {
    for (auto& x0 : *range_[1]) {
      // tensor label: Gamma60
      std::unique_ptr<double[]> i0data = in(0)->get_block(x3, x0, x2, x1);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, x0, x2, x1)]);
      sort_indices<0,1,2,3,0,1,1,1>(i0data, i0data_sorted, x3.size(), x0.size(), x2.size(), x1.size());
      // tensor label: I696
      std::unique_ptr<double[]> i1data = in(1)->get_block(x0, x3);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, x3)]);
      sort_indices<1,0,0,1,1,1>(i1data, i1data_sorted, x0.size(), x3.size());
      dgemm_("T", "N", x2.size()*x1.size(), 1, x0.size()*x3.size(),
             1.0, i0data_sorted, x0.size()*x3.size(), i1data_sorted, x0.size()*x3.size(),
             1.0, odata_sorted, x2.size()*x1.size());
    }
  }
  sort_indices<0,1,1,1,1,1>(odata_sorted, odata, x2.size(), x1.size());
  out()->put_block(odata, x2, x1);
}

void Task534::Task_local::compute() {
  const Index x0 = b(0);
  const Index x3 = b(1);
  // tensor label: I696
  std::unique_ptr<double[]> odata = out()->move_block(x0, x3);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x0, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x0, x3), 0.0);
  for (auto& a3 : *range_[2]) {
    for (auto& c2 : *range_[0]) {
      for (auto& a1 : *range_[2]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a3, c2, a1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, a3, c2, a1)]);
        sort_indices<1,2,3,0,0,1,1,1>(i0data, i0data_sorted, x3.size(), a3.size(), c2.size(), a1.size());
        // tensor label: I697
        std::unique_ptr<double[]> i1data = in(1)->get_block(a3, c2, a1, x0);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a3, c2, a1, x0)]);
        sort_indices<0,1,2,3,0,1,1,1>(i1data, i1data_sorted, a3.size(), c2.size(), a1.size(), x0.size());
        dgemm_("T", "N", x3.size(), x0.size(), a3.size()*c2.size()*a1.size(),
               1.0, i0data_sorted, a3.size()*c2.size()*a1.size(), i1data_sorted, a3.size()*c2.size()*a1.size(),
               1.0, odata_sorted, x3.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x3.size(), x0.size());
  out()->put_block(odata, x0, x3);
}

void Task535::Task_local::compute() {
  const Index a3 = b(0);
  const Index c2 = b(1);
  const Index a1 = b(2);
  const Index x0 = b(3);
  // tensor label: I697
  std::unique_ptr<double[]> odata = out()->move_block(a3, c2, a1, x0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, c2, a3);
    sort_indices<3,2,1,0,1,1,-1,1>(i0data, odata, x0.size(), a1.size(), c2.size(), a3.size());
  }
  out()->put_block(odata, a3, c2, a1, x0);
}

void Task536::Task_local::compute() {
  const Index x0 = b(0);
  const Index x3 = b(1);
  // tensor label: I696
  std::unique_ptr<double[]> odata = out()->move_block(x0, x3);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x0, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x0, x3), 0.0);
  for (auto& a1 : *range_[2]) {
    for (auto& c2 : *range_[0]) {
      for (auto& a3 : *range_[2]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a1, c2, a3);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, a1, c2, a3)]);
        sort_indices<1,2,3,0,0,1,1,1>(i0data, i0data_sorted, x3.size(), a1.size(), c2.size(), a3.size());
        // tensor label: I700
        std::unique_ptr<double[]> i1data = in(1)->get_block(a3, c2, a1, x0);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a3, c2, a1, x0)]);
        sort_indices<2,1,0,3,0,1,1,1>(i1data, i1data_sorted, a3.size(), c2.size(), a1.size(), x0.size());
        dgemm_("T", "N", x3.size(), x0.size(), a3.size()*c2.size()*a1.size(),
               1.0, i0data_sorted, a3.size()*c2.size()*a1.size(), i1data_sorted, a3.size()*c2.size()*a1.size(),
               1.0, odata_sorted, x3.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x3.size(), x0.size());
  out()->put_block(odata, x0, x3);
}

void Task537::Task_local::compute() {
  const Index a3 = b(0);
  const Index c2 = b(1);
  const Index a1 = b(2);
  const Index x0 = b(3);
  // tensor label: I700
  std::unique_ptr<double[]> odata = out()->move_block(a3, c2, a1, x0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, c2, a3);
    sort_indices<3,2,1,0,1,1,2,1>(i0data, odata, x0.size(), a1.size(), c2.size(), a3.size());
  }
  out()->put_block(odata, a3, c2, a1, x0);
}

void Task538::Task_local::compute() {
  const Index c4 = b(0);
  const Index c1 = b(1);
  // tensor label: r
  std::unique_ptr<double[]> odata = out()->move_block(c4, c1);
  {
    // tensor label: I492
    std::unique_ptr<double[]> i0data = in(0)->get_block(c1, c4);
    sort_indices<1,0,1,1,1,1>(i0data, odata, c1.size(), c4.size());
  }
  out()->put_block(odata, c4, c1);
}

void Task539::Task_local::compute() {
  const Index c1 = b(0);
  const Index c4 = b(1);
  // tensor label: I492
  std::unique_ptr<double[]> odata = out()->move_block(c1, c4);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c1, c4)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c1, c4), 0.0);
  for (auto& a2 : *range_[2]) {
    for (auto& c3 : *range_[0]) {
      for (auto& x1 : *range_[1]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c4, a2, c3, x1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c4, a2, c3, x1)]);
        sort_indices<1,2,3,0,0,1,1,1>(i0data, i0data_sorted, c4.size(), a2.size(), c3.size(), x1.size());
        // tensor label: I493
        std::unique_ptr<double[]> i1data = in(1)->get_block(c3, a2, c1, x1);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c3, a2, c1, x1)]);
        sort_indices<1,0,3,2,0,1,1,1>(i1data, i1data_sorted, c3.size(), a2.size(), c1.size(), x1.size());
        dgemm_("T", "N", c4.size(), c1.size(), c3.size()*a2.size()*x1.size(),
               1.0, i0data_sorted, c3.size()*a2.size()*x1.size(), i1data_sorted, c3.size()*a2.size()*x1.size(),
               1.0, odata_sorted, c4.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, c4.size(), c1.size());
  out()->put_block(odata, c1, c4);
}

void Task540::Task_local::compute() {
  const Index c3 = b(0);
  const Index a2 = b(1);
  const Index c1 = b(2);
  const Index x1 = b(3);
  // tensor label: I493
  std::unique_ptr<double[]> odata = out()->move_block(c3, a2, c1, x1);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, a2, c1, x1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, a2, c1, x1), 0.0);
  for (auto& x0 : *range_[1]) {
    // tensor label: Gamma16
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, x1);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, x1)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, x0.size(), x1.size());
    // tensor label: I494
    std::unique_ptr<double[]> i1data = in(1)->get_block(x0, c3, a2, c1);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, c3, a2, c1)]);
    sort_indices<0,1,2,3,0,1,1,1>(i1data, i1data_sorted, x0.size(), c3.size(), a2.size(), c1.size());
    dgemm_("T", "N", x1.size(), c3.size()*a2.size()*c1.size(), x0.size(),
           1.0, i0data_sorted, x0.size(), i1data_sorted, x0.size(),
           1.0, odata_sorted, x1.size());
  }
  sort_indices<1,2,3,0,1,1,1,1>(odata_sorted, odata, x1.size(), c3.size(), a2.size(), c1.size());
  out()->put_block(odata, c3, a2, c1, x1);
}

void Task541::Task_local::compute() {
  const Index x0 = b(0);
  const Index c3 = b(1);
  const Index a2 = b(2);
  const Index c1 = b(3);
  // tensor label: I494
  std::unique_ptr<double[]> odata = out()->move_block(x0, c3, a2, c1);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x0);
    sort_indices<3,2,1,0,1,1,-2,1>(i0data, odata, c1.size(), a2.size(), c3.size(), x0.size());
  }
  out()->put_block(odata, x0, c3, a2, c1);
}

void Task542::Task_local::compute() {
  const Index c1 = b(0);
  const Index c4 = b(1);
  // tensor label: I492
  std::unique_ptr<double[]> odata = out()->move_block(c1, c4);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c1, c4)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c1, c4), 0.0);
  for (auto& c3 : *range_[0]) {
    for (auto& a2 : *range_[2]) {
      for (auto& x1 : *range_[1]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c3, a2, c4, x1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c3, a2, c4, x1)]);
        sort_indices<0,1,3,2,0,1,1,1>(i0data, i0data_sorted, c3.size(), a2.size(), c4.size(), x1.size());
        // tensor label: I496
        std::unique_ptr<double[]> i1data = in(1)->get_block(c3, a2, c1, x1);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c3, a2, c1, x1)]);
        sort_indices<0,1,3,2,0,1,1,1>(i1data, i1data_sorted, c3.size(), a2.size(), c1.size(), x1.size());
        dgemm_("T", "N", c4.size(), c1.size(), c3.size()*a2.size()*x1.size(),
               1.0, i0data_sorted, c3.size()*a2.size()*x1.size(), i1data_sorted, c3.size()*a2.size()*x1.size(),
               1.0, odata_sorted, c4.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, c4.size(), c1.size());
  out()->put_block(odata, c1, c4);
}

void Task543::Task_local::compute() {
  const Index c3 = b(0);
  const Index a2 = b(1);
  const Index c1 = b(2);
  const Index x1 = b(3);
  // tensor label: I496
  std::unique_ptr<double[]> odata = out()->move_block(c3, a2, c1, x1);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, a2, c1, x1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, a2, c1, x1), 0.0);
  for (auto& x0 : *range_[1]) {
    // tensor label: Gamma16
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, x1);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, x1)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, x0.size(), x1.size());
    // tensor label: I497
    std::unique_ptr<double[]> i1data = in(1)->get_block(x0, c3, a2, c1);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, c3, a2, c1)]);
    sort_indices<0,1,2,3,0,1,1,1>(i1data, i1data_sorted, x0.size(), c3.size(), a2.size(), c1.size());
    dgemm_("T", "N", x1.size(), c3.size()*a2.size()*c1.size(), x0.size(),
           1.0, i0data_sorted, x0.size(), i1data_sorted, x0.size(),
           1.0, odata_sorted, x1.size());
  }
  sort_indices<1,2,3,0,1,1,1,1>(odata_sorted, odata, x1.size(), c3.size(), a2.size(), c1.size());
  out()->put_block(odata, c3, a2, c1, x1);
}

void Task544::Task_local::compute() {
  const Index x0 = b(0);
  const Index c3 = b(1);
  const Index a2 = b(2);
  const Index c1 = b(3);
  // tensor label: I497
  std::unique_ptr<double[]> odata = out()->move_block(x0, c3, a2, c1);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x0);
    sort_indices<3,2,1,0,1,1,1,1>(i0data, odata, c1.size(), a2.size(), c3.size(), x0.size());
  }
  out()->put_block(odata, x0, c3, a2, c1);
}

void Task545::Task_local::compute() {
  const Index c4 = b(0);
  const Index c3 = b(1);
  // tensor label: r
  std::unique_ptr<double[]> odata = out()->move_block(c4, c3);
  {
    // tensor label: I498
    std::unique_ptr<double[]> i0data = in(0)->get_block(c3, c4);
    sort_indices<1,0,1,1,1,1>(i0data, odata, c3.size(), c4.size());
  }
  out()->put_block(odata, c4, c3);
}

void Task546::Task_local::compute() {
  const Index c3 = b(0);
  const Index c4 = b(1);
  // tensor label: I498
  std::unique_ptr<double[]> odata = out()->move_block(c3, c4);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, c4)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, c4), 0.0);
  for (auto& a2 : *range_[2]) {
    for (auto& c1 : *range_[0]) {
      for (auto& x1 : *range_[1]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c4, a2, c1, x1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c4, a2, c1, x1)]);
        sort_indices<1,2,3,0,0,1,1,1>(i0data, i0data_sorted, c4.size(), a2.size(), c1.size(), x1.size());
        // tensor label: I499
        std::unique_ptr<double[]> i1data = in(1)->get_block(c3, a2, c1, x1);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c3, a2, c1, x1)]);
        sort_indices<1,2,3,0,0,1,1,1>(i1data, i1data_sorted, c3.size(), a2.size(), c1.size(), x1.size());
        dgemm_("T", "N", c4.size(), c3.size(), a2.size()*c1.size()*x1.size(),
               1.0, i0data_sorted, a2.size()*c1.size()*x1.size(), i1data_sorted, a2.size()*c1.size()*x1.size(),
               1.0, odata_sorted, c4.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, c4.size(), c3.size());
  out()->put_block(odata, c3, c4);
}

void Task547::Task_local::compute() {
  const Index c3 = b(0);
  const Index a2 = b(1);
  const Index c1 = b(2);
  const Index x1 = b(3);
  // tensor label: I499
  std::unique_ptr<double[]> odata = out()->move_block(c3, a2, c1, x1);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, a2, c1, x1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, a2, c1, x1), 0.0);
  for (auto& x0 : *range_[1]) {
    // tensor label: Gamma16
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, x1);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, x1)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, x0.size(), x1.size());
    // tensor label: I500
    std::unique_ptr<double[]> i1data = in(1)->get_block(x0, c3, a2, c1);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, c3, a2, c1)]);
    sort_indices<0,1,2,3,0,1,1,1>(i1data, i1data_sorted, x0.size(), c3.size(), a2.size(), c1.size());
    dgemm_("T", "N", x1.size(), c3.size()*a2.size()*c1.size(), x0.size(),
           1.0, i0data_sorted, x0.size(), i1data_sorted, x0.size(),
           1.0, odata_sorted, x1.size());
  }
  sort_indices<1,2,3,0,1,1,1,1>(odata_sorted, odata, x1.size(), c3.size(), a2.size(), c1.size());
  out()->put_block(odata, c3, a2, c1, x1);
}

void Task548::Task_local::compute() {
  const Index x0 = b(0);
  const Index c3 = b(1);
  const Index a2 = b(2);
  const Index c1 = b(3);
  // tensor label: I500
  std::unique_ptr<double[]> odata = out()->move_block(x0, c3, a2, c1);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, x0);
    sort_indices<3,2,1,0,1,1,1,1>(i0data, odata, c1.size(), a2.size(), c3.size(), x0.size());
  }
  out()->put_block(odata, x0, c3, a2, c1);
}

void Task549::Task_local::compute() {
  const Index c3 = b(0);
  const Index c4 = b(1);
  // tensor label: I498
  std::unique_ptr<double[]> odata = out()->move_block(c3, c4);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, c4)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, c4), 0.0);
  for (auto& c1 : *range_[0]) {
    for (auto& a2 : *range_[2]) {
      for (auto& x1 : *range_[1]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c4, x1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c4, x1)]);
        sort_indices<0,1,3,2,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c4.size(), x1.size());
        // tensor label: I505
        std::unique_ptr<double[]> i1data = in(1)->get_block(c3, a2, c1, x1);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c3, a2, c1, x1)]);
        sort_indices<2,1,3,0,0,1,1,1>(i1data, i1data_sorted, c3.size(), a2.size(), c1.size(), x1.size());
        dgemm_("T", "N", c4.size(), c3.size(), a2.size()*c1.size()*x1.size(),
               1.0, i0data_sorted, a2.size()*c1.size()*x1.size(), i1data_sorted, a2.size()*c1.size()*x1.size(),
               1.0, odata_sorted, c4.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, c4.size(), c3.size());
  out()->put_block(odata, c3, c4);
}

#endif
