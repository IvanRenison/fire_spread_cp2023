#include <cassert>

#include "fires.hpp"
#include "ignition_cells.hpp"
#include "landscape.hpp"
#include "many_simulations.hpp"
#include "spread_functions.hpp"

int main() {
  // read the landscape
  std::cout << "Reading the landscape" << std::endl;
  Landscape landscape("mini_landscape");

  // read the ignition cells
  std::cout << "Reading the ignition cells" << std::endl;
  IgnitionCells ignition_cells = read_ignition_cells("mini_landscape-ignition_points.csv");

  SimulationParams params = { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };

  splitmix64 rng_splitmix64(10);

  Xoshiro256plus rng(rng_splitmix64);

  // simulate de fire
  std::cout << "Simulating the fire" << std::endl;
  Fire s_fire = simulate_fire(rng, landscape, ignition_cells, params, 30, 1163.3, 399.5, 0.5);

  // Load the read file
  std::cout << "Reading the original fire" << std::endl;
  Fire r_fire(5, 5, "mini_landscape");

  // Check that the two fires are equal
  std::cout << "Checking that the two fires are equal" << std::endl;
  assert(s_fire == r_fire);

  // Check fire stats
  std::cout << "Checking fire stats" << std::endl;
  FireStats s_fire_stats = get_fire_stats(s_fire, landscape);
  assert(0 == s_fire_stats.counts_veg_subalpine);
  assert(0 == s_fire_stats.counts_veg_wet);
  assert(1 == s_fire_stats.counts_veg_dry);
  assert(0 == s_fire_stats.counts_veg_matorral);

  // Test compare fires
  std::cout << "Testing compare fires" << std::endl;
  FireStats r_fire_stats = get_fire_stats(r_fire, landscape);
  compare_result s_result = compare_fires(s_fire, s_fire_stats, r_fire, r_fire_stats);

  // Compare with result in original version
  assert(abs(s_result.overlap_sp - 1.0) < 0.000001);
  assert(abs(s_result.overlap_vd - 1.0) < 0.000001);
  assert(abs(s_result.overlap_norm - 1.0) < 0.000001);
  assert(abs(s_result.overlap_expquad - 1.0) < 0.000001);
  assert(abs(s_result.overlap_quad - 1.0) < 0.000001);
  assert(abs(s_result.sp_norm_5050 - 1.0) < 0.000001);
  assert(abs(s_result.sp_norm_7525 - 1.0) < 0.000001);
  assert(abs(s_result.sp_expquad_5050 - 1.0) < 0.000001);
  assert(abs(s_result.sp_expquad_7525 - 1.0) < 0.000001);
  assert(abs(s_result.sp_quad_5050 - 1.0) < 0.000001);
  assert(abs(s_result.sp_quad_7525 - 1.0) < 0.000001);

  // Test emulate loglik
  std::cout << "Testing emulate loglik (with many particles)" << std::endl;

  std::vector<SimulationParams> particles = { { 0.1, 0, 0.1, 0.1, 0.01, 0.1, 0.1, 0.1, 0.11 },
                                              { 0.001, 0.1, 0.1, 0.1, 0.2, 0.1, 0.1, 0.1,
                                                0.08 } };

  // Set seed
  std::srand(20);

  std::vector<std::vector<compare_result>> many_particles_result = emulate_loglik(
      landscape, ignition_cells, particles, 30, 1163.3, 399.5, 0.5, r_fire, r_fire_stats, 2
  );

  // Compare with result in original version
  assert(abs(many_particles_result[0][0].overlap_sp - 0.823529) < 0.000001);
  assert(abs(many_particles_result[0][0].overlap_vd - 0.920168) < 0.000001);
  assert(abs(many_particles_result[0][0].overlap_norm - 0.918269) < 0.000001);
  assert(abs(many_particles_result[0][0].overlap_expquad - 0.967152) < 0.000001);
  assert(abs(many_particles_result[0][0].overlap_quad - 0.99332) < 0.000001);
  assert(abs(many_particles_result[0][0].sp_norm_5050 - 0.870899) < 0.000001);
  assert(abs(many_particles_result[0][0].sp_norm_7525 - 0.847214) < 0.000001);
  assert(abs(many_particles_result[0][0].sp_expquad_5050 - 0.895341) < 0.000001);
  assert(abs(many_particles_result[0][0].sp_expquad_7525 - 0.859435) < 0.000001);
  assert(abs(many_particles_result[0][0].sp_quad_5050 - 0.908425) < 0.000001);
  assert(abs(many_particles_result[0][0].sp_quad_7525 - 0.865977) < 0.000001);
  assert(abs(many_particles_result[0][1].overlap_sp - 0.45) < 0.000001);
  assert(abs(many_particles_result[0][1].overlap_vd - 0.909524) < 0.000001);
  assert(abs(many_particles_result[0][1].overlap_norm - 0.909341) < 0.000001);
  assert(abs(many_particles_result[0][1].overlap_expquad - 0.959737) < 0.000001);
  assert(abs(many_particles_result[0][1].overlap_quad - 0.991781) < 0.000001);
  assert(abs(many_particles_result[0][1].sp_norm_5050 - 0.67967) < 0.000001);
  assert(abs(many_particles_result[0][1].sp_norm_7525 - 0.564835) < 0.000001);
  assert(abs(many_particles_result[0][1].sp_expquad_5050 - 0.704869) < 0.000001);
  assert(abs(many_particles_result[0][1].sp_expquad_7525 - 0.577434) < 0.000001);
  assert(abs(many_particles_result[0][1].sp_quad_5050 - 0.72089) < 0.000001);
  assert(abs(many_particles_result[0][1].sp_quad_7525 - 0.585445) < 0.000001);
  assert(abs(many_particles_result[1][0].overlap_sp - 0.190476) < 0.000001);
  assert(abs(many_particles_result[1][0].overlap_vd - 0.753247) < 0.000001);
  assert(abs(many_particles_result[1][0].overlap_norm - 0.839286) < 0.000001);
  assert(abs(many_particles_result[1][0].overlap_expquad - 0.878846) < 0.000001);
  assert(abs(many_particles_result[1][0].overlap_quad - 0.974171) < 0.000001);
  assert(abs(many_particles_result[1][0].sp_norm_5050 - 0.514881) < 0.000001);
  assert(abs(many_particles_result[1][0].sp_norm_7525 - 0.352679) < 0.000001);
  assert(abs(many_particles_result[1][0].sp_expquad_5050 - 0.534661) < 0.000001);
  assert(abs(many_particles_result[1][0].sp_expquad_7525 - 0.362569) < 0.000001);
  assert(abs(many_particles_result[1][0].sp_quad_5050 - 0.582324) < 0.000001);
  assert(abs(many_particles_result[1][0].sp_quad_7525 - 0.3864) < 0.000001);
  assert(abs(many_particles_result[1][1].overlap_sp - 0.368421) < 0.000001);
  assert(abs(many_particles_result[1][1].overlap_vd - 0.904762) < 0.000001);
  assert(abs(many_particles_result[1][1].overlap_norm - 0.95) < 0.000001);
  assert(abs(many_particles_result[1][1].overlap_expquad - 0.987578) < 0.000001);
  assert(abs(many_particles_result[1][1].overlap_quad - 0.9975) < 0.000001);
  assert(abs(many_particles_result[1][1].sp_norm_5050 - 0.659211) < 0.000001);
  assert(abs(many_particles_result[1][1].sp_norm_7525 - 0.513816) < 0.000001);
  assert(abs(many_particles_result[1][1].sp_expquad_5050 - 0.677999) < 0.000001);
  assert(abs(many_particles_result[1][1].sp_expquad_7525 - 0.52321) < 0.000001);
  assert(abs(many_particles_result[1][1].sp_quad_5050 - 0.682961) < 0.000001);
  assert(abs(many_particles_result[1][1].sp_quad_7525 - 0.525691) < 0.000001);

  std::cout << "All tests passed" << std::endl;

  return EXIT_SUCCESS;
}
