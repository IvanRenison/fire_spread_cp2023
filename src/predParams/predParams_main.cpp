#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "csv.hpp"
#include "predParams.hpp"
#include "spread_functions.hpp"

#define SEED 10
#define DISTANCE 30
#define ELEVATION_MEAN 1163.3
#define ELEVATION_SD 399.5
#define UPPER_LIMIT 0.5

std::vector<SimulationParams> read_simulationParams_csv(std::string filename) {
  std::vector<SimulationParams> particles;

  std::ifstream in_file(filename);

  CSVIterator csv_in(in_file);

  ++csv_in; // Skip header

  for (; csv_in != CSVIterator(); ++csv_in) {
    if ((*csv_in).size() < 9) {
      throw std::runtime_error("Invalid params csv file " + filename);
    }
    SimulationParams particle;
    particle.independent_pred = atof((*csv_in)[0].data());
    particle.wind_pred = atof((*csv_in)[1].data());
    particle.elevation_pred = atof((*csv_in)[2].data());
    particle.slope_pred = atof((*csv_in)[3].data());
    particle.subalpine_pred = atof((*csv_in)[4].data());
    particle.wet_pred = atof((*csv_in)[5].data());
    particle.dry_pred = atof((*csv_in)[6].data());
    particle.fwi_pred = atof((*csv_in)[7].data());
    particle.aspect_pred = atof((*csv_in)[8].data());

    particles.push_back(particle);
  }

  return particles;
}

int main(int argc, char** argv) {

  // Parse command line arguments
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0]
              << " <file with list of landscapes> <csv with list of particles> <ouput_folder>"
              << std::endl;
    return EXIT_FAILURE;
  }

  try {

    std::string landscapes_filename = argv[1];
    std::string particles_filename = argv[2];
    std::string output_foldername = argv[3];

    std::vector<SimulationParams> particles = read_simulationParams_csv(particles_filename);

    std::ifstream landscapes_file(landscapes_filename);
    std::vector<std::string> landscapes;

    while (!landscapes_file.eof()) {
      // Read line
      std::string line;
      std::getline(landscapes_file, line);

      landscapes.push_back(line);
    }

    emulate_many_logliks(
        landscapes, output_foldername, particles, DISTANCE, ELEVATION_MEAN, ELEVATION_SD,
        UPPER_LIMIT, 10
    );

  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
