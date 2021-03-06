//
// BAGEL - Parallel electron correlation program.
// Filename: test_scf.cc
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

#include <sstream>
#include <src/scf/hf/rhf.h>
#include <src/scf/hf/rohf.h>
#include <src/scf/hf/uhf.h>
#include <src/scf/sohf/soscf.h>
#include <src/wfn/reference.h>

using namespace bagel;

double scf_energy(std::string filename, std::string extension = ".json") {
  auto ofs = std::make_shared<std::ofstream>(filename + ".testout", std::ios::trunc);
  std::streambuf* backup_stream = std::cout.rdbuf(ofs->rdbuf());

  // a bit ugly to hardwire an input file, but anyway...
  std::stringstream ss; ss << location__ << filename << extension;
  auto idata = std::make_shared<const PTree>(ss.str());
  auto keys = idata->get_child("bagel");
  std::shared_ptr<Geometry> geom;
  std::shared_ptr<const Reference> ref;

  for (auto& itree : *keys) {
    const std::string method = to_lower(itree->get<std::string>("title", ""));

    if (method == "molecule") {
      geom = geom ? std::make_shared<Geometry>(*geom, itree) : std::make_shared<Geometry>(itree);
      if (ref) ref = ref->project_coeff(geom);
    } else if (method == "hf") {
      auto scf = std::make_shared<RHF>(itree, geom, ref);
      scf->compute();
      ref = scf->conv_to_ref();
    } else if (method == "uhf") {
      auto scf = std::make_shared<UHF>(itree, geom, ref);
      scf->compute();
      ref = scf->conv_to_ref();
    } else if (method == "rohf") {
      auto scf = std::make_shared<ROHF>(itree, geom, ref);
      scf->compute();
      ref = scf->conv_to_ref();
    } else if (method == "soscf") {
      auto scf = std::make_shared<SOSCF>(itree, geom, ref);
      scf->compute();
      ref = scf->conv_to_ref();
#ifndef DISABLE_SERIALIZATION
    } else if (method == "continue") {
      IArchive archive(itree->get<std::string>("archive"));
      Method* ptr;
      archive >> ptr;
      auto scf = std::shared_ptr<Method>(ptr);
      scf->compute();
      ref = scf->conv_to_ref();
#endif
    } else {
      throw std::logic_error("unknown method in scf test");
    }
  }
  std::cout.rdbuf(backup_stream);
  return ref->energy();
}

BOOST_AUTO_TEST_SUITE(TEST_SCF)

BOOST_AUTO_TEST_CASE(DF_HF) {
    BOOST_CHECK(compare(scf_energy("hf_svp_hf"),          -99.84779026));
    BOOST_CHECK(compare(scf_energy("hf_svp_dfhf"),        -99.84772354));
#ifndef DISABLE_SERIALIZATION
//  BOOST_CHECK(compare(scf_energy("hf_svp_dfhf_restart"),-99.84772354));
#endif
    BOOST_CHECK(compare(scf_energy("hf_svp_dfhf",".bgl"), -99.84772354));
    BOOST_CHECK(compare(scf_energy("hf_svp_dfhf_ext"),    -99.83765614));
    BOOST_CHECK(compare(scf_energy("hf_svp_dfhf_cart"),   -99.84911270));
    BOOST_CHECK(compare(scf_energy("hf_svp_dfhf_charge"), -99.78567137));
    BOOST_CHECK(compare(scf_energy("hf_mix_dfhf"),        -99.83889193));
    BOOST_CHECK(compare(scf_energy("hf_mix2_dfhf"),       -99.83889193));
    BOOST_CHECK(compare(scf_energy("oh_svp_uhf"),         -75.28410147));
    BOOST_CHECK(compare(scf_energy("hc_svp_rohf"),        -38.16810629));
    BOOST_CHECK(compare(scf_energy("hf_new_dfhf"),        -99.97989929));
    BOOST_CHECK(compare(scf_energy("hcl_svp_dfhf"),      -459.93784632));
    BOOST_CHECK(compare(scf_energy("cuh2_ecp_hf"),       -196.12254012));
    BOOST_CHECK(compare(scf_energy("hbr_ecp_sohf"),       -13.68431370));
}

BOOST_AUTO_TEST_SUITE_END()
