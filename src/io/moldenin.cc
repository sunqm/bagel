//
// BAGEL - Parallel electron correlation program.
// Filename: moldenin.cc
// Copyright (C) 2012 Shane Parker
//
// Author: Shane Parker <shane.parker@u.northwestern.edu>
// Maintainer: NU theory
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and\/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
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

#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>

#include <fstream>
#include <vector>

#include <src/io/moldenin.h>
#include <src/scf/atom.h>
#include <src/scf/geometry.h>
#include <src/wfn/reference.h>
#include <src/util/constants.h>
#include <src/scf/atommap.h>

using namespace bagel;
using namespace std;

/************************************************************************************
************************************************************************************/

MoldenIn::MoldenIn(const string filename, const bool is_spherical) : MoldenIO(filename), is_spherical_(is_spherical) {
  compute_transforms();

}

MoldenIn& MoldenIn::operator>> (vector<shared_ptr<const Atom> >& atoms) {
  atoms = atoms_;

  return *this;
}

MoldenIn& MoldenIn::operator>> (shared_ptr<const Coeff>& coeff) {
  assert(!mo_coefficients_.empty());

  vector<int> atom_offsets;
  for(auto& ioff : coeff->geom()->offsets()) {
    atom_offsets.push_back(ioff.front());
  }

  const int num_basis = coeff->geom()->nbasis();

  cout << shell_orders_.size() << endl;

  double *idata = coeff->data();
  for(auto& imo : mo_coefficients_) {
    auto icoeff = imo.begin();
    int ii = 0;
    for(auto iatom = shell_orders_.begin(); iatom != shell_orders_.end(); ++iatom, ++ii) {
      double* tmp_idata = idata + atom_offsets[gto_order_[ii]-1];
      for(auto& ishell : *iatom) {
        if (cartesian_) {
          vector<int> corder = m2b_cart_.at(ishell);
          vector<double> scales = scaling_.at(ishell);
          int jj = 0;
          if (is_spherical_) {
            vector<double> in;
            for(auto& iorder : corder)
              in.push_back(*(icoeff + iorder) * scales.at(jj++));
            vector<double> new_in = transform_cart(in, ishell);
            tmp_idata = copy(new_in.begin(), new_in.end(), tmp_idata);
          }
          else {
            for(auto& iorder : corder)
              *tmp_idata++ = *(icoeff + iorder) * scales.at(jj++);
          }
          icoeff += corder.size();
        }
        else {
          vector<int> corder = m2b_sph_.at(ishell);
          for(auto& iorder : corder)
            *tmp_idata++ = *(icoeff + iorder);
          icoeff += corder.size();
        }
      }
    }
    idata += num_basis;
  }

  return *this;
}

MoldenIn& MoldenIn::operator>> (shared_ptr<const Reference>& ref) {
  shared_ptr<const Coeff> coeff = ref->coeff();
  *this >> coeff;

  return *this;
}

void MoldenIn::read() {
  /************************************************************
  *  Set up variables that will contain the organized info    *
  ************************************************************/
  int num_atoms = 0;

  /* Atom positions */
  vector< array<double,3> > positions;
  /* Atom names */
  vector< string > names;
  /* Map atom number to basis info */
  map<int, vector<tuple<string, vector<double>, vector<double> > > > basis_info;

  /************************************************************
  *  Set up "global" regular expressions                      *
  ************************************************************/
  boost::regex gto_re("\\[GTO\\]");
  boost::regex atoms_re("\\[Atoms\\]");
  boost::regex mo_re("\\[MO\\]");
  boost::regex other_re("\\[\\w+\\]");

  boost::cmatch matches;

  /************************************************************
  *  Booleans to check and make sure each important section   *
  *  was found                                                *
  ************************************************************/
  bool found_atoms = false;
  bool found_gto = false;
  bool found_mo = false;

  /************************************************************
  * Extra variables                                           *
  ************************************************************/
  double scale;
  string line; // Contains the current line of the file

  /************************************************************
  *  An inelegant search to check for the 5D keyword          *
  ************************************************************/
  cartesian_ = true;

  ifstream sph_input;
  sph_input.open(filename_.c_str());
  if(!sph_input.is_open()){
    throw runtime_error("Molden input file not found");
  }   
  else {
    boost::regex _5d_re("\\[5[Dd]\\]");
    boost::regex _5d7f_re("\\[5[Dd]7[Ff]\\]");
    while (!sph_input.eof()) {
      getline(sph_input, line);
      if(boost::regex_search(line,_5d_re)){
        cartesian_ = false;
      }   
      else if(boost::regex_search(line,_5d7f_re)) {
        cartesian_ = false;
      }   
    }   
  }   
  sph_input.close();

  /************************************************************
  *  Open input stream                                        *
  ************************************************************/

  ifs_.open(filename_.c_str());

  getline(ifs_, line);

  while (!ifs_.eof()){
    if (boost::regex_search(line,atoms_re)) {
      boost::regex ang_re("Angs");
      boost::regex atoms_line("(\\w{1,2})\\s+\\d+\\s+\\d+\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)");

      scale = boost::regex_search(line, ang_re) ? ang2bohr__ : 1.0;

      getline(ifs_, line);
      while(!boost::regex_search(line, other_re)){
        if (ifs_.eof()) { break; }

        if(boost::regex_search(line.c_str(), matches, atoms_line)) {
          ++num_atoms;

          const string nm(matches[1].first, matches[1].second);
          names.push_back(nm);
          
          const string x_str(matches[2].first, matches[2].second);
          const string y_str(matches[3].first, matches[3].second);
          const string z_str(matches[4].first, matches[4].second);

          array<double,3> pos;

          pos[0] = boost::lexical_cast<double>(x_str)*scale;
          pos[1] = boost::lexical_cast<double>(y_str)*scale;
          pos[2] = boost::lexical_cast<double>(z_str)*scale;
          
          positions.push_back(pos);

          getline(ifs_,line);
        }
        else { getline(ifs_,line); }
      }

      found_atoms = true;
    }
    else if (boost::regex_search(line,gto_re)){
      getline(ifs_, line);

      boost::regex atom_line("(\\d+)\\s*\\S*");
      boost::regex shell_line("([spdf])\\s+(\\d+)\\s*\\S*");
      boost::regex exp_line("(\\S+)\\s+(\\S+)");
      boost::regex Dd("[Dd]");

      while(!boost::regex_search(line,other_re)){
        if (ifs_.eof()) { break; }

        /* This line should be a new atom */
        if(!boost::regex_search(line.c_str(), matches, atom_line)) {
           getline(ifs_,line); continue;
        }
        vector<tuple<string,vector<double>,vector<double> > > atom_basis_info;

        const string atom_no_str(matches[1].first, matches[1].second);
        const int atom_no = boost::lexical_cast<int>(atom_no_str.c_str());

        gto_order_.push_back(atom_no);

        vector<int> atomic_shell_order;
        AtomMap atommap;

        getline(ifs_, line);

        while(boost::regex_search(line.c_str(), matches, shell_line)) {
          /* Now it should be a new angular shell */
          const string ang_l(matches[1].first, matches[1].second);
          atomic_shell_order.push_back(atommap.angular_number(ang_l));

          const string num_exp_string(matches[2].first, matches[2].second);
          const int num_exponents = boost::lexical_cast<int>(num_exp_string);

          vector<double> exponents;
          vector<double> coefficients;

          for (int i = 0; i < num_exponents; ++i) {
            getline(ifs_,line);

            boost::regex_search(line.c_str(), matches, exp_line);

            string exp_string(matches[1].first, matches[1].second);
            string coeff_string(matches[2].first, matches[2].second);

            exp_string = boost::regex_replace(exp_string, Dd, "E");
            coeff_string = boost::regex_replace(coeff_string, Dd, "E");

            const double exponent = boost::lexical_cast<double>(exp_string);
            const double coeff = boost::lexical_cast<double>(coeff_string);

            exponents.push_back(exponent);
            coefficients.push_back(coeff);
          }

          /************************************************************
          *  Right now I have evec and cvec which have the newly      *
          *  exponents and coefficients                               *
          ************************************************************/
          atom_basis_info.push_back(make_tuple(ang_l, exponents, coefficients));

          getline(ifs_, line); 
        }
        //atom_basis_info to basis_info
        shell_orders_.push_back(atomic_shell_order);
        basis_info.insert(pair<int,vector<tuple<string,vector<double>,vector<double> > > >(atom_no, atom_basis_info));
      }

      found_gto = true;
    }
    else if (boost::regex_search(line,mo_re)) {
      if(!found_gto) {
        throw runtime_error("MO section found before GTO section. Check Molden file.");
      }
      /* Not used at the moment. Maybe later.
      boost::regex sym_re("Sym=\\s+(\\S+)");
      boost::regex ene_re("Ene=\\s+(\\S+)");
      boost::regex spin_re("Spin=\\s+(\\w+)");
      boost::regex occup_re("Occup=\\s+(\\S+)"); */
      boost::regex coeff_re("\\d+\\s+(\\S+)");

      getline(ifs_,line);
      while(!boost::regex_search(line,other_re)) {
        if (ifs_.eof()) { break; }

        vector<double> movec;

        getline(ifs_,line);
        while(!boost::regex_search(line.c_str(),coeff_re)) {
          /* For now, throwing away excess data until we get to MO coefficients */
          getline(ifs_,line);
        }

        while(boost::regex_search(line.c_str(),matches,coeff_re)){
          string mo_string(matches[1].first, matches[1].second);
          double coeff = boost::lexical_cast<double>(mo_string);

          movec.push_back(coeff);
          getline(ifs_,line);
        }

        mo_coefficients_.push_back(movec);
      }
      found_mo = true;
    }
    else {
      getline(ifs_,line);
    }
  }

  /************************************************************
  *  Check to make sure all the necessary information was     *
  *  found.                                                   *
  ************************************************************/
  if( !(found_atoms && found_gto) ){
     string message("Section not found in Molden file: ");
     if (!found_atoms){ message += "atoms "; }
     if (!found_gto)  { message += "GTO"; }
     throw runtime_error(message);
  }
  
  vector<shared_ptr<const Atom> > all_atoms;
  
  /* Assuming the names and positions vectors are in the right order */
  vector<string>::iterator iname = names.begin();
  vector<array<double,3> >::iterator piter = positions.begin();
  for(int i = 0; i < num_atoms; ++i, ++iname, ++piter){
     vector<tuple<string, vector<double>, vector<double> > > binfo = basis_info.find(i+1)->second;
     if (i == num_atoms) {
        throw runtime_error("It appears an atom was missing in the GTO section. Check your file");
     }
  
     transform(iname->begin(), iname->end(), iname->begin(), ::tolower);
     /* For each atom, I need to make an atom object and stick it into a vector */
     shared_ptr<const Atom> this_atom(new Atom(is_spherical_, *iname, *piter, binfo));
  
     all_atoms.push_back(this_atom);
  }

  atoms_ = all_atoms;

  ifs_.close();
}