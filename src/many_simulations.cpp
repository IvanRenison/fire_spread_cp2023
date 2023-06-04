#include "many_simulations.hpp"
#include "xoshiro256plus.hpp"

#include <assert.h>
#include <cmath>
#include <omp.h>

#ifdef BENCHMARKING_OMP
#include "wtime.hpp"
#endif

#ifndef SEED
#define SEED 10
#endif

compare_result compare_fires(
    const Fire& fire1, const FireStats& fire1stats, const Fire& fire2,
    const FireStats& fire2stats, float lscale
) {

  assert(fire1.width == fire2.width && fire1.height == fire2.height);

  // Extract list elements ------------------------------------------------

  Matrix<char> burned1 = fire1.burned_layer;
  Matrix<char> burned2 = fire2.burned_layer;

  std::vector<std::pair<uint, uint>> burned_ids1 = fire1.burned_ids;
  std::vector<std::pair<uint, uint>> burned_ids2 = fire2.burned_ids;

  float size1 = burned_ids1.size();
  float size2 = burned_ids2.size();

  uint common = 0;
  // compute common pixels only in the smaller fire
  if (size1 <= size2) {
    for (int i = 0; i < size1; i++) {
      common += burned2[burned_ids1[i].first, burned_ids1[i].second];
    }
  } else {
    for (int i = 0; i < size2; i++) {
      common += burned1[burned_ids2[i].first, burned_ids2[i].second];
    }
  }

  float overlap_sp = (float)common / (float)(size1 + size2 - common);

  // overlap_vd -----------------------------------------------------------
  // compute vegetation distribution overlap
  float overlap_vd = 0.0;
  overlap_vd += std::min(
      (float)fire1stats.counts_veg_matorral / (float)size1,
      (float)fire2stats.counts_veg_matorral / (float)size2
  );
  overlap_vd += std::min(
      (float)fire1stats.counts_veg_subalpine / (float)size1,
      (float)fire2stats.counts_veg_subalpine / (float)size2
  );
  overlap_vd += std::min(
      (float)fire1stats.counts_veg_wet / (float)size1,
      (float)fire2stats.counts_veg_wet / (float)size2
  );
  overlap_vd += std::min(
      (float)fire1stats.counts_veg_dry / (float)size1,
      (float)fire2stats.counts_veg_dry / (float)size2
  );

  // deltas by veg_type ---------------------------------------------------

  // normalized difference using absolute difference. The difference by veg_type
  // is in [0, 1]. So, if we divide delta_norm by 4 (veg_num), it will be in [0, 1].
  float delta_norm = 0.0;

  float sum_area_matorral =
      ((float)fire1stats.counts_veg_matorral + (float)fire2stats.counts_veg_matorral);
  if (sum_area_matorral > 0.0) {
    delta_norm += std::abs(
        ((float)fire1stats.counts_veg_matorral - (float)fire2stats.counts_veg_matorral) /
        sum_area_matorral
    );
  }
  float sum_area_subalpine =
      ((float)fire1stats.counts_veg_subalpine + (float)fire2stats.counts_veg_subalpine);
  if (sum_area_subalpine > 0.0) {
    delta_norm += std::abs(
        ((float)fire1stats.counts_veg_subalpine - (float)fire2stats.counts_veg_subalpine) /
        sum_area_subalpine
    );
  }
  float sum_area_wet = ((float)fire1stats.counts_veg_wet + (float)fire2stats.counts_veg_wet);
  if (sum_area_wet > 0.0) {
    delta_norm += std::abs(
        ((float)fire1stats.counts_veg_wet - (float)fire2stats.counts_veg_wet) / sum_area_wet
    );
  }
  float sum_area_dry = ((float)fire1stats.counts_veg_dry + (float)fire2stats.counts_veg_dry);
  if (sum_area_dry > 0.0) {
    delta_norm += std::abs(
        ((float)fire1stats.counts_veg_dry - (float)fire2stats.counts_veg_dry) / sum_area_dry
    );
  }

  // Scale to [0, 1]
  float delta_norm_unit = delta_norm / 4;

  // Transform to similarities
  float overlap_norm = 1.0 - delta_norm_unit;
  float overlap_expquad = exp(-pow(delta_norm_unit, 2.0) / lscale); // 0.2 is the Gaussian SD.
  float overlap_quad = 1 - pow(delta_norm_unit, 2.0);

  // ---------------------------------------------------------------------

  compare_result indexes = { // pure indices
                             .overlap_sp = overlap_sp,

                             .overlap_vd = overlap_vd,
                             .overlap_norm = overlap_norm,
                             .overlap_expquad = overlap_expquad,
                             .overlap_quad = overlap_quad,

                             // mixture indices
                             .sp_norm_5050 = (0.50f * overlap_sp + 0.50f * overlap_norm),
                             .sp_norm_7525 = (0.75f * overlap_sp + 0.25f * overlap_norm),
                             .sp_expquad_5050 = (0.50f * overlap_sp + 0.50f * overlap_expquad),
                             .sp_expquad_7525 = (0.75f * overlap_sp + 0.25f * overlap_expquad),
                             .sp_quad_5050 = (0.50f * overlap_sp + 0.50f * overlap_quad),
                             .sp_quad_7525 = (0.75f * overlap_sp + 0.25f * overlap_quad)
  };
#ifdef BENCHMARKING_OMP
  indexes.total_burning_size = fire2.burned_ids.size();
#endif
  return indexes;
}

std::vector<compare_result> emulate_loglik_particle(
    Xoshiro256plus& rng, const Landscape& landscape,
    const std::vector<std::pair<uint, uint>>& ignition_cells, const SimulationParams params,
    float distance, float elevation_mean, float elevation_sd, float upper_limit,
    const Fire& fire_ref, const FireStats& fire_ref_stats, int n_replicates
) {

  std::vector<compare_result> similarity(n_replicates);

  for (int i = 0; i < n_replicates; i++) {

    // simulate_fire
    Fire fire_sim = simulate_fire(
        rng, landscape, ignition_cells, params, distance, elevation_mean, elevation_sd,
        upper_limit
    );

    similarity[i] =
        compare_fires(fire_ref, fire_ref_stats, fire_sim, get_fire_stats(fire_sim, landscape));
  }

  return similarity;
}

std::vector<std::vector<compare_result>> emulate_loglik(
    const Landscape& landscape, const std::vector<std::pair<uint, uint>>& ignition_cells,
    const std::vector<SimulationParams>& particles, float distance, float elevation_mean,
    float elevation_sd, float upper_limit, const Fire& fire_ref,
    const FireStats& fire_ref_stats, int n_replicates
) {

  splitmix64 rng_splitmix64(SEED);

  int n_particles = particles.size();

  std::vector<std::vector<compare_result>> similarity(n_particles);

#ifdef BENCHMARKING_OMP
  uint sum_total_burning_size = 0;
  double start_time, time_elapsed;
  start_time = wtime();
#endif
  #pragma omp parallel firstprivate(                                                           \
          n_replicates, n_particles, ignition_cells, distance, elevation_mean, elevation_sd,   \
              upper_limit, fire_ref, fire_ref_stats, particles                                 \
  ) shared(landscape, rng_splitmix64, similarity) default(none)
  {
    Xoshiro256plus rng(rng_splitmix64);

    #pragma omp for
    for (int part = 0; part < n_particles; part++) {
      similarity[part] = emulate_loglik_particle(
          rng, landscape, ignition_cells, particles[part], distance, elevation_mean,
          elevation_sd, upper_limit, fire_ref, fire_ref_stats, n_replicates
      );
    }
  }
#ifdef BENCHMARKING_OMP
  time_elapsed = wtime() - start_time;
  for (int part = 0; part < n_particles; part++) {
    for (int i = 0; i < n_replicates; i++) {
      sum_total_burning_size += similarity[part][i].total_burning_size;
    }
  }
  std::cout.precision(17);
  std::cout << sum_total_burning_size / time_elapsed << "," << sum_total_burning_size << ","
            << time_elapsed << std::endl;
#endif
  return similarity;
}

Matrix<uint> burned_amounts_per_cell(
    const Landscape& landscape, const std::vector<std::pair<uint, uint>>& ignition_cells,
    const SimulationParams& params, float distance, float elevation_mean, float elevation_sd,
    float upper_limit, uint n_replicates
) {

  splitmix64 rng_splitmix64(SEED);

  Xoshiro256plus rng(rng_splitmix64);

  Matrix<uint> burned_amounts(landscape.width, landscape.height);

  #pragma omp parallel firstprivate(                                                           \
          ignition_cells, params, distance, elevation_mean, elevation_sd, upper_limit,         \
              n_replicates                                                                     \
  ) shared(landscape, rng_splitmix64, burned_amounts) default(none)
  {
    Xoshiro256plus rng(rng_splitmix64);

    #pragma omp for
    for (uint i = 0; i < n_replicates; i++) {
      Fire fire = simulate_fire(
          rng, landscape, ignition_cells, params, distance, elevation_mean, elevation_sd,
          upper_limit
      );

      for (uint row = 0; row < landscape.width; row++) {
        for (uint col = 0; col < landscape.height; col++) {
          if (fire.burned_layer[row, col]) {
            burned_amounts[row, col] += 1;
          }
        }
      }
    }
  }

  return burned_amounts;
}
