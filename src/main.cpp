
#include "fires.hpp"
#include "ignition_cells.hpp"
#include "landscape.hpp"
#include "many_simulations.hpp"
#include "spread_functions.hpp"

#define SEED 10
#define N_PARTICLES 3

// main function reading command line arguments
int main(int argc, char* argv[]) {
  try {

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
    IgnitionCells ignition_cells =
        read_ignition_cells(landscape_file_prefix + "-ignition_points.csv");

    SimulationParams params = { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };

    // Set seed
    std::srand(SEED);

    // simulate de fire
    Fire fire = simulate_fire(landscape, ignition_cells, params, 30, 1163.3, 399.5, 0.5);

    FireStats fire_stats = get_fire_stats(fire, landscape);

    std::vector<SimulationParams> particles;
    for (size_t i = 0; i < N_PARTICLES; i++) {
      particles.push_back(random_params());
    }

    std::vector<std::vector<compare_result>> many_particles_result = emulate_loglik(
        landscape, ignition_cells, particles, 30, 1163.3, 399.5, 0.5, fire, fire_stats, 5
    );
    for (std::pair<uint, uint> id : fire.burned_ids) {
      std::cout << id.first << " " << id.second << std::endl;
    }

    // print the fire
    std::cout << fire.burned_ids.size() << std::endl;

  } catch (std::runtime_error& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
