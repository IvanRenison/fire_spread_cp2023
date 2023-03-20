
#include "fires.hpp"
#include "landscape.hpp"
#include "ignition_cells.hpp"
#include "spread_functions.hpp"

// main function reading command line arguments
int main(int argc, char* argv[]) {

  // check if the number of arguments is correct
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <landscape_file_prefix>" << std::endl;
    return EXIT_FAILURE;
  }

  // read the landscape file prefix
  std::string landscape_file_prefix = argv[1];

  // read the landscape
  Landscape landscape(landscape_file_prefix);

  // read the ignition cells
  IgnitionCells ignition_cells = read_ignition_cells(landscape_file_prefix + "-ignition_points.csv");

  SimulationParams params = {
    1000, 0, 0, 0, 0, 0, 0, 0, 0
  };

  // simulate de fire
  Fire fire = simulate_fire(landscape, ignition_cells, params, 30, 1163.3, 399.5, 1);

  // print the fire
  std::cout << fire.burned_ids.size() << std::endl;

  return EXIT_SUCCESS;
}
