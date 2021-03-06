//
// BAGEL - Parallel electron correlation program.
// Filename: MRCI_gen1.cc
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

#include <src/smith/MRCI_tasks1.h>

using namespace std;
using namespace bagel;
using namespace bagel::SMITH;
using namespace bagel::SMITH::MRCI;

Task0::Task0(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x4 : *range[1])
      for (auto& x0 : *range[1])
        for (auto& x5 : *range[1])
          for (auto& x2 : *range[1])
            for (auto& x3 : *range[1])
              subtasks_.push_back(make_shared<Task_local>(array<const Index,6>{{x5, x0, x4, x1, x3, x2}}, in, t[0], range));
}

Task1::Task1(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,3> in = {{t[1], t[2], t[3]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x6 : *range[1])
      for (auto& x0 : *range[1])
        for (auto& x7 : *range[1])
          for (auto& x2 : *range[1])
            for (auto& x3 : *range[1])
              for (auto& x4 : *range[1])
                for (auto& x5 : *range[1])
                  subtasks_.push_back(make_shared<Task_local>(array<const Index,8>{{x7, x0, x6, x1, x5, x4, x3, x2}}, in, t[0], range));
}

Task2::Task2(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x0 : *range[1])
    for (auto& x2 : *range[1])
      for (auto& x1 : *range[1])
        for (auto& x3 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x3, x1, x2, x0}}, in, t[0], range));
}

Task3::Task3(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x2 : *range[1])
      for (auto& x0 : *range[1])
        for (auto& x3 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x3, x0, x2, x1}}, in, t[0], range));
}

Task4::Task4(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x2 : *range[1])
    for (auto& x3 : *range[1])
      for (auto& x1 : *range[1])
        for (auto& x4 : *range[1])
          for (auto& x0 : *range[1])
            for (auto& x5 : *range[1])
              subtasks_.push_back(make_shared<Task_local>(array<const Index,6>{{x5, x0, x4, x1, x3, x2}}, in, t[0], range));
}

Task5::Task5(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x2 : *range[1])
      for (auto& x3 : *range[1])
        for (auto& x4 : *range[1])
          for (auto& x0 : *range[1])
            for (auto& x5 : *range[1])
              subtasks_.push_back(make_shared<Task_local>(array<const Index,6>{{x5, x0, x4, x3, x2, x1}}, in, t[0], range));
}

Task6::Task6(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x3 : *range[1])
      for (auto& x2 : *range[1])
        for (auto& x4 : *range[1])
          for (auto& x0 : *range[1])
            for (auto& x5 : *range[1])
              subtasks_.push_back(make_shared<Task_local>(array<const Index,6>{{x5, x0, x4, x2, x3, x1}}, in, t[0], range));
}

Task7::Task7(vector<shared_ptr<Tensor>> t) {
  r_ =  t[0];
}

Task8::Task8(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& a1 : *range[2])
    for (auto& x0 : *range[1])
      for (auto& a2 : *range[2])
        for (auto& x1 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x1, a2, x0, a1}}, in, t[0], range));
}

Task9::Task9(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[2]->nblock()*range[2]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& a2 : *range[2])
    for (auto& a1 : *range[2])
      for (auto& x1 : *range[1])
        for (auto& x0 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x0, x1, a1, a2}}, in, t[0], range));
}

Task10::Task10(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x4 : *range[1])
      for (auto& x0 : *range[1])
        for (auto& x5 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x5, x0, x4, x1}}, in, t[0], range));
}

Task11::Task11(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[2]->nblock()*range[2]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& a2 : *range[2])
    for (auto& a1 : *range[2])
      for (auto& x1 : *range[1])
        for (auto& x0 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x0, x1, a1, a2}}, in, t[0], range));
}

Task12::Task12(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& a2 : *range[2])
    for (auto& x6 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& x7 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x7, a1, x6, a2}}, in, t[0], range));
}

Task13::Task13(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[2]->nblock()*range[2]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& a2 : *range[2])
    for (auto& a1 : *range[2])
      for (auto& x1 : *range[1])
        for (auto& x0 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x0, x1, a1, a2}}, in, t[0], range));
}

Task14::Task14(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x2 : *range[1])
    for (auto& x3 : *range[1])
      for (auto& a2 : *range[2])
        for (auto& a1 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a1, a2, x3, x2}}, in, t[0], range));
}

Task15::Task15(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[2]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& a2 : *range[2])
    for (auto& a3 : *range[2])
      for (auto& a1 : *range[2])
        for (auto& a4 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a4, a1, a3, a2}}, in, t[0], range));
}

Task16::Task16(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& a1 : *range[2])
    for (auto& x0 : *range[1])
      for (auto& a2 : *range[2])
        for (auto& x1 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x1, a2, x0, a1}}, in, t[0], range));
}

Task17::Task17(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x0 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& a2 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a2, a1, x0, x1}}, in, t[0], range));
}

Task18::Task18(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[2]->nblock()*range[1]->nblock()*range[2]->nblock());
  for (auto& x2 : *range[1])
    for (auto& a1 : *range[2])
      for (auto& x3 : *range[1])
        for (auto& a2 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a2, x3, a1, x2}}, in, t[0], range));
}

Task19::Task19(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[2]->nblock());
  for (auto& a2 : *range[2])
    for (auto& a3 : *range[2])
      subtasks_.push_back(make_shared<Task_local>(array<const Index,2>{{a3, a2}}, in, t[0], range));
}

Task20::Task20(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x0 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& a2 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a2, a1, x0, x1}}, in, t[0], range));
}

Task21::Task21(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x2 : *range[1])
    for (auto& x3 : *range[1])
      for (auto& x1 : *range[1])
        for (auto& x0 : *range[1])
          for (auto& a3 : *range[2])
            for (auto& a1 : *range[2])
              subtasks_.push_back(make_shared<Task_local>(array<const Index,6>{{a1, a3, x0, x1, x3, x2}}, in, t[0], range));
}

Task22::Task22(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& a3 : *range[2])
    for (auto& x4 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& x5 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x5, a1, x4, a3}}, in, t[0], range));
}

Task23::Task23(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x0 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& a2 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a2, a1, x0, x1}}, in, t[0], range));
}

Task24::Task24(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x4 : *range[1])
    for (auto& a1 : *range[2])
      for (auto& x5 : *range[1])
        for (auto& a2 : *range[2])
          for (auto& x2 : *range[1])
            for (auto& x3 : *range[1])
              subtasks_.push_back(make_shared<Task_local>(array<const Index,6>{{x3, x2, a2, x5, a1, x4}}, in, t[0], range));
}

Task25::Task25(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[1]->nblock()*range[2]->nblock());
  for (auto& a2 : *range[2])
    for (auto& x2 : *range[1])
      for (auto& x3 : *range[1])
        for (auto& a3 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a3, x3, x2, a2}}, in, t[0], range));
}

Task26::Task26(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x0 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& a2 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a2, a1, x0, x1}}, in, t[0], range));
}

Task27::Task27(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[2]->nblock()*range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& x4 : *range[1])
    for (auto& a1 : *range[2])
      for (auto& x5 : *range[1])
        for (auto& x2 : *range[1])
          for (auto& a2 : *range[2])
            for (auto& x3 : *range[1])
              subtasks_.push_back(make_shared<Task_local>(array<const Index,6>{{x3, a2, x2, x5, a1, x4}}, in, t[0], range));
}

Task28::Task28(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[1]->nblock()*range[2]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& x2 : *range[1])
    for (auto& a3 : *range[2])
      for (auto& a2 : *range[2])
        for (auto& x3 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x3, a2, a3, x2}}, in, t[0], range));
}

Task29::Task29(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x0 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& a2 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a2, a1, x0, x1}}, in, t[0], range));
}

Task30::Task30(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& x4 : *range[1])
    for (auto& a1 : *range[2])
      for (auto& x5 : *range[1])
        for (auto& a2 : *range[2])
          for (auto& x2 : *range[1])
            for (auto& x3 : *range[1])
              subtasks_.push_back(make_shared<Task_local>(array<const Index,6>{{x3, x2, a2, x5, a1, x4}}, in, t[0], range));
}

Task31::Task31(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[2]->nblock()*range[1]->nblock()*range[1]->nblock());
  for (auto& a2 : *range[2])
    for (auto& a3 : *range[2])
      for (auto& x2 : *range[1])
        for (auto& x3 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x3, x2, a3, a2}}, in, t[0], range));
}

Task32::Task32(vector<shared_ptr<Tensor>> t) {
  s_ =  t[0];
}

Task33::Task33(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& a1 : *range[2])
    for (auto& x0 : *range[1])
      for (auto& a2 : *range[2])
        for (auto& x1 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x1, a2, x0, a1}}, in, t[0], range));
}

Task34::Task34(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x0 : *range[1])
      for (auto& a2 : *range[2])
        for (auto& a1 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a1, a2, x0, x1}}, in, t[0], range));
}

Task35::Task35(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& a2 : *range[2])
    for (auto& x2 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& x3 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x3, a1, x2, a2}}, in, t[0], range));
}

Task36::Task36(vector<shared_ptr<Tensor>> t) {
  n_ =  t[0];
}

Task37::Task37(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& a1 : *range[2])
    for (auto& x0 : *range[1])
      for (auto& a2 : *range[2])
        for (auto& x1 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x1, a2, x0, a1}}, in, t[0], range));
}

Task38::Task38(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,2> in = {{t[1], t[2]}};
  subtasks_.reserve(range[1]->nblock()*range[1]->nblock()*range[2]->nblock()*range[2]->nblock());
  for (auto& x1 : *range[1])
    for (auto& x0 : *range[1])
      for (auto& a2 : *range[2])
        for (auto& a1 : *range[2])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{a1, a2, x0, x1}}, in, t[0], range));
}

Task39::Task39(vector<shared_ptr<Tensor>> t, array<shared_ptr<const IndexRange>,3> range) {
  array<shared_ptr<const Tensor>,1> in = {{t[1]}};
  subtasks_.reserve(range[2]->nblock()*range[1]->nblock()*range[2]->nblock()*range[1]->nblock());
  for (auto& a2 : *range[2])
    for (auto& x2 : *range[1])
      for (auto& a1 : *range[2])
        for (auto& x3 : *range[1])
          subtasks_.push_back(make_shared<Task_local>(array<const Index,4>{{x3, a1, x2, a2}}, in, t[0], range));
}

#endif
