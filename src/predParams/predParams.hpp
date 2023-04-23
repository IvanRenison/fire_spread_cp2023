#ifndef PRED_PARAMS_HPP
#define PRED_PARAMS_HPP

#include <string>
#include <vector>

#include "spread_functions.hpp"

// Emulate one loglik reding fire data from a file and saving the results to a csv file
// The csv result file will have the columns:
// overlap_sp, overlap_vd, overlap_norm, overlap_expquad, overlap_quad,
// sp_norm_5050, sp_norm_7525, sp_expquad_5050, sp_expquad_7525, sp_quad_5050, sp_quad_7525
//
// And each row will be the result with a different particle, in the same order as the
// particles vector
void emulate_one_loglik(
    std::string files_prefix, std::string output_filename, std::vector<SimulationParams> params,
    float distance, float elevation_mean, float elevation_sd, float upper_limit,
    uint n_replicates = 10
);

// The same but for many files
void emulate_many_logliks(
    std::vector<std::string> files_prefixes, std::string output_folder,
    std::vector<SimulationParams> particles, float distance, float elevation_mean,
    float elevation_sd, float upper_limit, uint n_replicates = 10
);

#endif