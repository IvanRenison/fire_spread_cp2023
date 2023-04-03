#include "many_simulations.hpp"

#include <assert.h>
#include <cmath>

compare_result compare_fires(
    Fire fire1, FireStats fire1stats, Fire fire2, FireStats fire2stats, double lscale
) {

  assert(fire1.width == fire2.width && fire1.height == fire2.height);

  // Extract list elements ------------------------------------------------

  Matrix<char> burned1 = fire1.burned_layer;
  Matrix<char> burned2 = fire2.burned_layer;

  std::vector<std::pair<uint, uint>> burned_ids1 = fire1.burned_ids;
  std::vector<std::pair<uint, uint>> burned_ids2 = fire2.burned_ids;

  double size1 = burned_ids1.size();
  double size2 = burned_ids2.size();

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

  double overlap_sp = (double)common / (double)(size1 + size2 - common);

  // overlap_vd -----------------------------------------------------------
  // compute vegetation distribution overlap
  double overlap_vd = 0.0;
  overlap_vd += std::min(
      (double)fire1stats.counts_veg_matorral / (double)size1,
      (double)fire2stats.counts_veg_matorral / (double)size2
  );
  overlap_vd += std::min(
      (double)fire1stats.counts_veg_subalpine / (double)size1,
      (double)fire2stats.counts_veg_subalpine / (double)size2
  );
  overlap_vd += std::min(
      (double)fire1stats.counts_veg_wet / (double)size1,
      (double)fire2stats.counts_veg_wet / (double)size2
  );
  overlap_vd += std::min(
      (double)fire1stats.counts_veg_dry / (double)size1,
      (double)fire2stats.counts_veg_dry / (double)size2
  );

  // deltas by veg_type ---------------------------------------------------

  // normalized difference using absolute difference. The difference by veg_type
  // is in [0, 1]. So, if we divide delta_norm by 4 (veg_num), it will be in [0, 1].
  double delta_norm = 0.0;

  double sum_area_matorral =
      ((double)fire1stats.counts_veg_matorral + (double)fire2stats.counts_veg_matorral);
  if (sum_area_matorral > 0.0) {
    delta_norm += std::abs(
        ((double)fire1stats.counts_veg_matorral - (double)fire2stats.counts_veg_matorral) /
        sum_area_matorral
    );
  }
  double sum_area_subalpine =
      ((double)fire1stats.counts_veg_subalpine + (double)fire2stats.counts_veg_subalpine);
  if (sum_area_subalpine > 0.0) {
    delta_norm += std::abs(
        ((double)fire1stats.counts_veg_subalpine - (double)fire2stats.counts_veg_subalpine) /
        sum_area_subalpine
    );
  }
  double sum_area_wet = ((double)fire1stats.counts_veg_wet + (double)fire2stats.counts_veg_wet);
  if (sum_area_wet > 0.0) {
    delta_norm += std::abs(
        ((double)fire1stats.counts_veg_wet - (double)fire2stats.counts_veg_wet) / sum_area_wet
    );
  }
  double sum_area_dry = ((double)fire1stats.counts_veg_dry + (double)fire2stats.counts_veg_dry);
  if (sum_area_dry > 0.0) {
    delta_norm += std::abs(
        ((double)fire1stats.counts_veg_dry - (double)fire2stats.counts_veg_dry) / sum_area_dry
    );
  }

  // Scale to [0, 1]
  double delta_norm_unit = delta_norm / 4;

  // Transform to similarities
  double overlap_norm = 1.0 - delta_norm_unit;
  double overlap_expquad = exp(-pow(delta_norm_unit, 2.0) / lscale); // 0.2 is the Gaussian SD.
  double overlap_quad = 1 - pow(delta_norm_unit, 2.0);

  // ---------------------------------------------------------------------

  compare_result indexes = { // pure indices
                             .overlap_sp = overlap_sp,

                             .overlap_vd = overlap_vd,
                             .overlap_norm = overlap_norm,
                             .overlap_expquad = overlap_expquad,
                             .overlap_quad = overlap_quad,

                             // mixture indices
                             .sp_norm_5050 = (0.50 * overlap_sp + 0.50 * overlap_norm),
                             .sp_norm_7525 = (0.75 * overlap_sp + 0.25 * overlap_norm),
                             .sp_expquad_5050 = (0.50 * overlap_sp + 0.50 * overlap_expquad),
                             .sp_expquad_7525 = (0.75 * overlap_sp + 0.25 * overlap_expquad),
                             .sp_quad_5050 = (0.50 * overlap_sp + 0.50 * overlap_quad),
                             .sp_quad_7525 = (0.75 * overlap_sp + 0.25 * overlap_quad)
  };

  return indexes;
}

std::vector<compare_result> emulate_loglik_particle(
    Landscape landscape, std::vector<std::pair<uint, uint>> ignition_cells,
    SimulationParams params, double distance, double elevation_mean, double elevation_sd,
    double upper_limit, Fire fire_ref, FireStats fire_ref_stats, int n_replicates
) {

  std::vector<compare_result> similarity(n_replicates);

  for (int i = 0; i < n_replicates; i++) {

    // simulate_fire
    Fire fire_sim = simulate_fire(
        landscape, ignition_cells, params, distance, elevation_mean, elevation_sd, upper_limit
    );

    similarity[i] =
        compare_fires(fire_ref, fire_ref_stats, fire_sim, get_fire_stats(fire_sim, landscape));
  }

  return similarity;
}

std::vector<std::vector<compare_result>> emulate_loglik(
    Landscape landscape, std::vector<std::pair<uint, uint>> ignition_cells,
    std::vector<SimulationParams> particles, double distance, double elevation_mean,
    double elevation_sd, double upper_limit, Fire fire_ref, FireStats fire_ref_stats,
    int n_replicates
) {

  int n_particles = particles.size();

  std::vector<std::vector<compare_result>> similarity(n_particles);

  for (int part = 0; part < n_particles; part++) {
    similarity[part] = emulate_loglik_particle(
        landscape, ignition_cells, particles[part], distance, elevation_mean, elevation_sd,
        upper_limit, fire_ref, fire_ref_stats, n_replicates
    );
  }

  return similarity;
}