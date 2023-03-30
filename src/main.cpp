
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
    if (argc != 4) {
      std::cout << "Usage: " << argv[0] << " <landscape_file_prefix>" << std::endl;
      return EXIT_FAILURE;
    }

    // read the landscape file prefix
    std::string landscape_file_prefix = argv[1];
    uint n_particles = atoi(argv[2]);
    uint m_same_particles = atoi(argv[3]);

    // read the landscape
    Landscape landscape(landscape_file_prefix);

    // read the ignition cells
    IgnitionCells ignition_cells =
        read_ignition_cells(landscape_file_prefix + "-ignition_points.csv");

    // Set seed
    std::srand(SEED);

    // read de fire
    Fire fire(landscape.width, landscape.height, landscape_file_prefix);

    FireStats fire_stats = get_fire_stats(fire, landscape);

    std::vector<SimulationParams> particles(n_particles);
    for (size_t i = 0; i < n_particles; i++) {
      particles[i] = random_params();
    }

    std::vector<std::vector<compare_result>> many_particles_result = emulate_loglik(
        landscape, ignition_cells, particles, DISTANCE, ELEVATION_MEAN, ELEVATION_SD, UPPER_LIMIT, fire, fire_stats,
        m_same_particles
    );

  } catch (std::runtime_error& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
