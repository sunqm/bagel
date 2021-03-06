//
// BAGEL - Parallel electron correlation program.
// Filename: asd_dmrg.h
// Copyright (C) 2014 Shane Parker
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

#ifndef __ASD_DMRG_ASD_DMRG_H
#define __ASD_DMRG_ASD_DMRG_H

#include <src/asd/multisite/multisite.h>
#include <src/asd/dmrg/dmrg_block.h>

namespace bagel {

/// Base class for DMRG using ASD
class ASD_DMRG {
  protected:
    /// Input block given to ASD_DMRG
    std::shared_ptr<const PTree> input_;
    /// contains orbital information
    std::shared_ptr<const MultiSite> multisite_;

    /// DMRG_Block representing L block containing l sites is in left_blocks_[l-1]
    std::vector<std::shared_ptr<DMRG_Block1>> left_blocks_;
    /// DMRG_Block representing R block containing l sites is in right_blocks_[l-1]
    std::vector<std::shared_ptr<DMRG_Block1>> right_blocks_;

    std::vector<double> weights_; ///< weights to use when building RDM

    std::vector<std::vector<double>> sweep_energies_; ///< Stores the energies of each state for each step of the sweep
    std::vector<double> energies_; ///< final energies

    int nsites_;  ///< Number of sites in the DMRG model
    int nstate_;  ///< Number of states to target
    int maxiter_; ///< Maximum number of full sweeps to perform
    int ntrunc_;  ///< Number of states to keep in each DMRG block. Same as \f$M\f$ in the DMRG literature

    double thresh_; ///< convergence threshold for initial portion of calculation
    double perturb_; ///< magnitude of perturbation added to RDM
    double perturb_thresh_; ///< threshold at which to decrease the perturbation
    double perturb_min_; ///< minimum value of perturbation (below this, it is just set to zero)

    double down_thresh_; ///< convergence threshold for sweeping downwards with smaller M. Should probably be tighter than thresh_
    bool down_sweep_; ///< controls whether to sweep with decreasing values of ntrunc_ after the main calculation
    std::vector<int> down_sweep_truncs_; ///< descending list of values to use for ntrunc_

    /// Prints graphical depiction of sweep process, mainly probably useful for debugging
    std::string print_progress(const int position, const std::string left_symbol, const std::string right_symbol) const;

    /// Kicks off by doing a CAS/RAS calculation in the first site with the rest of the sites at mean-field
    virtual std::shared_ptr<DMRG_Block1> compute_first_block(std::vector<std::shared_ptr<PTree>> inputs, std::shared_ptr<const Reference> ref) = 0;
    /// Adds one site to the block
    virtual std::shared_ptr<DMRG_Block1> grow_block(std::vector<std::shared_ptr<PTree>> inputs, std::shared_ptr<const Reference> ref, std::shared_ptr<DMRG_Block1> left, const int site) = 0;
    /** Performs one step in the sweep by adding one site to the system block
        @param input input describing the desired total system wavefunction
        @param ref one-particle reference for site
        @param system block to be grown
        @param environment block being decimated
    */
    virtual std::shared_ptr<DMRG_Block1> decimate_block(std::shared_ptr<PTree> input, std::shared_ptr<const Reference> ref, std::shared_ptr<DMRG_Block1> system, std::shared_ptr<DMRG_Block1> environment, const int site) = 0;

  public:
    /// Unlike MEH classes, ASD_DMRG will also be the driver for CI calculations
    ASD_DMRG(const std::shared_ptr<const PTree> input, std::shared_ptr<const MultiSite> multisite);

    /// Driver for calculation
    void compute();

    /// runs calculations for smaller values of M after the main calculation has finished
    void down_sweep();

    const std::vector<double>& energies() const { return energies_; }
    double energies(const int i) const { return energies_.at(i); }

  private:
    /// Prepare several input files used for growing the chain
    std::vector<std::shared_ptr<PTree>> prepare_growing_input(const int site) const;
    /// Prepare one input to be used during the sweep
    std::shared_ptr<PTree> prepare_sweeping_input(const int site) const;
};

}

#endif
