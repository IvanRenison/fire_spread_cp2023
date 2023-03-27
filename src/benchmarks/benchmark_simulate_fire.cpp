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

  std::srand(10);
  Fire fire = simulate_fire(landscape, ignition_cells, params, 30, 1163.3, 399.5, 1);

  if (fire.burned_ids.size() != 1134656) {
    std::cerr << "Error: the number of burned cells is not correct" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
