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

  // Set seed
  std::srand(10);

  // simulate de fire
  std::cout << "Simulating the fire" << std::endl;
  Fire s_fire = simulate_fire(landscape, ignition_cells, params, 30, 1163.3, 399.5, 0.5);

  // Load the read file
  std::cout << "Reading the original fire" << std::endl;
  Fire r_fire = read_fire(5, 5, "mini_landscape");

  // Check that the two fires are equal
  std::cout << "Checking that the two fires are equal" << std::endl;
  assert(s_fire == r_fire);

  // Check fire stats
  std::cout << "Checking fire stats" << std::endl;
  FireStats s_fire_stats = get_fire_stats(s_fire, landscape);
  assert(6 == s_fire_stats.counts_veg_subalpine);
  assert(3 == s_fire_stats.counts_veg_wet);
  assert(1 == s_fire_stats.counts_veg_dry);
  assert(4 == s_fire_stats.counts_veg_matorral);

  // Test compare fires
  std::cout << "Testing compare fires" << std::endl;
  FireStats r_fire_stats = get_fire_stats(r_fire, landscape);
  compare_result s_result = compare_fires(s_fire, s_fire_stats, r_fire, r_fire_stats);

  assert(abs(s_result.overlap_sp - 1.0) < 0.0001);
  assert(abs(s_result.overlap_vd - 1.0) < 0.0001);
  assert(abs(s_result.overlap_norm - 1.0) < 0.0001);
  assert(abs(s_result.overlap_expquad - 1.0) < 0.0001);
  assert(abs(s_result.overlap_quad - 1.0) < 0.0001);
  assert(abs(s_result.sp_norm_5050 - 1.0) < 0.0001);
  assert(abs(s_result.sp_norm_7525 - 1.0) < 0.0001);
  assert(abs(s_result.sp_expquad_5050 - 1.0) < 0.0001);
  assert(abs(s_result.sp_expquad_7525 - 1.0) < 0.0001);
  assert(abs(s_result.sp_quad_5050 - 1.0) < 0.0001);
  assert(abs(s_result.sp_quad_7525 - 1.0) < 0.0001);

  std::cout << "All tests passed" << std::endl;

  return EXIT_SUCCESS;
}
