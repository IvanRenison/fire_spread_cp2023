#ifndef GRAPHICS
#define GRAPHICS
#endif

#include "fires.hpp"
#include "ignition_cells.hpp"
#include "landscape.hpp"
#include "many_simulations.hpp"
#include "spread_functions.hpp"

#define SEED 10
#define DISTANCE 30
#define ELEVATION_MEAN 1163.3
#define ELEVATION_SD 399.5
#define UPPER_LIMIT 0.5

// main function reading command line arguments
int main(int argc, char* argv[]) {
  try {

    // check if the number of arguments is correct
    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <landscape_file_prefix>" << std::endl;
      return EXIT_FAILURE;
    }

    // read the landscape file prefix
    std::string landscape_file_prefix = argv[1];

    // read the landscape
    Landscape landscape(landscape_file_prefix);

    // read the ignition cells
    IgnitionCells ignition_cells =
        read_ignition_cells(landscape_file_prefix + "-ignition_points.csv");

    std::srand(SEED);
    Fire fire = simulate_fire(
        landscape, ignition_cells, random_params(), DISTANCE, ELEVATION_MEAN, ELEVATION_SD,
        UPPER_LIMIT
    );

    // Print the fire

    std::cout << "Landscape size: " << landscape.width << " " << landscape.height << std::endl;

    uint step = 0;
    uint i = 0;
    for (uint j : fire.burned_ids_steps) {
        if (i >= j) {
            continue;
        }
        std::cout << "Step " << step << ":" << std::endl;
        for (; i < j; i++) {
            std::cout << fire.burned_ids[i].first << " " << fire.burned_ids[i].second << std::endl;
        }
        step++;
    }

  } catch (std::runtime_error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
