#include "predParams.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "fires.hpp"
#include "ignition_cells.hpp"
#include "landscape.hpp"
#include "many_simulations.hpp"

void emulate_one_loglik(
    std::string filename_prefix, std::string output_folder,
    std::vector<SimulationParams> params, float distance, float elevation_mean,
    float elevation_sd, float upper_limit, uint n_replicates
) {

  Landscape landscape(filename_prefix);

  // get filename from filename_prefix without the path
  std::string fire_name = filename_prefix.substr(filename_prefix.find_last_of("/") + 1);

  std::string output_filename = output_folder + "/" + fire_name + "-loglikResult.csv";
  std::ifstream in_file(output_filename);

  if (!in_file) {
    // File does not exist, create it
    in_file.close();
    std::ofstream out_file(output_filename);
    out_file.close();
  }

  // Check that file is empty
  if (in_file.peek() != std::ifstream::traits_type::eof()) {
    throw std::runtime_error("File " + output_filename + " already has contents");
  }

  // Close file, we will open it again later but for writing
  in_file.close();

  uint width = landscape.width;
  uint height = landscape.height;

  IgnitionCells ignition_cells = read_ignition_cells(filename_prefix + "-ignition_points.csv");

  Fire fire_ref(width, height, filename_prefix);

  FireStats fire_ref_stats = get_fire_stats(fire_ref, landscape);

  std::vector<compare_result> results = emulate_loglik_average(
      landscape, ignition_cells, params, distance, elevation_mean, elevation_sd, upper_limit,
      fire_ref, fire_ref_stats, n_replicates
  );

  // Write results to file
  std::ofstream out_file(output_filename);

  out_file
      << "\"overlap_sp\",\"overlap_vd\",\"overlap_norm\",\"overlap_expquad\",\"overlap_quad\","
      << "\"sp_norm_5050\",\"sp_norm_7525\",\"sp_expquad_5050\",\"sp_expquad_7525\","
      << "\"sp_quad_5050\",\"sp_quad_7525\"" << std::endl;

  for (compare_result result : results) {
    out_file << result.overlap_sp << "," << result.overlap_vd << "," << result.overlap_norm
             << "," << result.overlap_expquad << "," << result.overlap_quad << ","
             << result.sp_norm_5050 << "," << result.sp_norm_7525 << ","
             << result.sp_expquad_5050 << "," << result.sp_expquad_7525 << ","
             << result.sp_quad_5050 << "," << result.sp_quad_7525 << std::endl;
  }

  out_file.close();
}

void emulate_many_logliks(
    std::vector<std::string> files_prefixes, std::string output_folder,
    std::vector<SimulationParams> particles, float distance, float elevation_mean,
    float elevation_sd, float upper_limit, uint n_replicates
) {
  for (std::string filename_prefix : files_prefixes) {
    std::cout << "Emulating loglik for " << filename_prefix << std::endl;
    emulate_one_loglik(
        filename_prefix, output_folder, particles, distance, elevation_mean, elevation_sd,
        upper_limit, n_replicates
    );
    std::cout << "Done" << std::endl;
  }
}