#ifndef _MANY_SIMULATIONS_HPP
#define _MANY_SIMULATIONS_HPP

#include <vector>

#include "fires.hpp"
#include "landscape.hpp"
#include "spread_functions.hpp"

typedef struct _s_compare_result {
  double overlap_sp;

  double overlap_vd;
  double overlap_norm;
  double overlap_expquad;
  double overlap_quad;

  double sp_norm_5050;
  double sp_norm_7525;
  double sp_expquad_5050;
  double sp_expquad_7525;
  double sp_quad_5050;
  double sp_quad_7525;
} compare_result;

// Function compare two fires using many similarity indexes, to try them as proxies for the
// likelihood, when simulated fires are compared to the observed one. "_try" because it
// computes many similarity indexes; after selecting one we will have a function to compute
// only the best one.
compare_result compare_fires(
    Fire fire1, FireStats fire1stats, Fire fire2, FireStats fire2stats, double lscale = 0.2
);

// Function to emulate the spread model's likelihood, approximated by many similarity indexes
// between the observed and simulated fires. It's a wrapper around simulate_fire_compare, which
// returns the simulated fire with data useful for comparison. In addition, it takes as
// arguments data from the observed fire to be compared.
std::vector<compare_result> emulate_loglik_particle(
    Landscape landscape, std::vector<std::pair<uint, uint>> ignition_cells,
    SimulationParams params, double distance, double elevation_mean, double elevation_sd,
    double upper_limit, Fire fire_ref, FireStats fire_ref_stats, int n_replicates = 10
);

// The same function but evaluating many particles all at once
std::vector<std::vector<compare_result>> emulate_loglik(
    Landscape landscape, std::vector<std::pair<uint, uint>> ignition_cells,
    std::vector<SimulationParams> particles, double distance, double elevation_mean,
    double elevation_sd, double upper_limit, Fire fire_ref, FireStats fire_ref_stats,
    int n_replicates = 10
);

#endif