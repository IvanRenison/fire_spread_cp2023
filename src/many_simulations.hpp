#ifndef _MANY_SIMULATIONS_HPP
#define _MANY_SIMULATIONS_HPP

#include <vector>

#include "fires.hpp"
#include "landscape.hpp"
#include "spread_functions.hpp"
#include "xoshiro256plus.hpp"

typedef struct _s_compare_result {
  float overlap_sp;

  float overlap_vd;
  float overlap_norm;
  float overlap_expquad;
  float overlap_quad;

  float sp_norm_5050;
  float sp_norm_7525;
  float sp_expquad_5050;
  float sp_expquad_7525;
  float sp_quad_5050;
  float sp_quad_7525;
} compare_result;

// Function compare two fires using many similarity indexes, to try them as proxies for the
// likelihood, when simulated fires are compared to the observed one. "_try" because it
// computes many similarity indexes; after selecting one we will have a function to compute
// only the best one.
compare_result compare_fires(
    const Fire& fire1, const FireStats& fire1stats, const Fire& fire2,
    const FireStats& fire2stats, float lscale = 0.2
);

// Function to emulate the spread model's likelihood, approximated by many similarity indexes
// between the observed and simulated fires. It's a wrapper around simulate_fire_compare, which
// returns the simulated fire with data useful for comparison. In addition, it takes as
// arguments data from the observed fire to be compared.
std::vector<compare_result> emulate_loglik_particle(
    Xoshiro256plus& rng, const Landscape& landscape,
    const std::vector<std::pair<uint, uint>>& ignition_cells, const SimulationParams params,
    float distance, float elevation_mean, float elevation_sd, float upper_limit,
    const Fire& fire_ref, const FireStats& fire_ref_stats, int n_replicates = 10
);

// The same function but evaluating many particles all at once
std::vector<std::vector<compare_result>> emulate_loglik(
    const Landscape& landscape, const std::vector<std::pair<uint, uint>>& ignition_cells,
    const std::vector<SimulationParams>& particles, float distance, float elevation_mean,
    float elevation_sd, float upper_limit, const Fire& fire_ref,
    const FireStats& fire_ref_stats, int n_replicates = 10
);

// Make `n_replicates` simulations and return a matrix in witch each element represents the
// amount of simulation in which the corresponding cell was burned.
Matrix<uint> burned_amounts_per_cell(
    const Landscape& landscape, const std::vector<std::pair<uint, uint>>& ignition_cells,
    const SimulationParams& params, float distance, float elevation_mean, float elevation_sd,
    float upper_limit, uint n_replicates
);

#endif