#include <cassert>

#include "fires.hpp"
#include "ignition_cells.hpp"
#include "landscape.hpp"
#include "many_simulations.hpp"
#include "spread_functions.hpp"

int main(void) {

  Landscape landscape("1999_27j_S");
  IgnitionCells ignition_cells = read_ignition_cells("1999_27j_S-ignition_points.csv");
  SimulationParams params = { 1000, 1, 1, 1, 1, 1, 1, 1, 1 };

  std::srand(35);
  std::vector<SimulationParams> particles(10);
  for (size_t i = 0; i < 10; i++) {
    particles[i] = random_params();
  }

  Fire fire(landscape.width, landscape.height, "1999_27j_S");
  FireStats fire_stats = get_fire_stats(fire, landscape);

  std::vector<std::vector<compare_result>> many_particles_result = emulate_loglik(
      landscape, ignition_cells, particles, 30, 1163.3, 399.5, 0.5, fire, fire_stats, 2
  );

  if (many_particles_result.size() != 10) {
    std::cerr << "Error: the number of burned cells is not correct" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
