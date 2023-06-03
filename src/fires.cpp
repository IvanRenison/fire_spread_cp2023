#include "fires.hpp"

#include "landscape.hpp"
#include "matrix.hpp"

Fire::Fire(uint width, uint height, std::string filename_prefix)
    : Fire(width, height, Matrix<char>(width, height), std::vector<std::pair<uint, uint>>()) {
  this->width = width;
  this->height = height;

  std::ifstream burned_ids_file(filename_prefix + "-burned_ids.csv");

  if (!burned_ids_file.is_open()) {
    throw std::runtime_error("Can't open landscape file");
  }

  CSVIterator loop(burned_ids_file);
  loop++;

  burned_ids.reserve(width * height);

  for (; loop != CSVIterator(); ++loop) {
    if (loop->size() < 2) {
      throw std::runtime_error("Invalid fire file");
    }
    uint x = atoi((*loop)[0].data());
    uint y = atoi((*loop)[1].data());
    if (x >= width || y >= height) {
      throw std::runtime_error("Invalid fire file");
    }
    burned_layer[x, y] = true;
    burned_ids.push_back({ x, y });
  }
#ifdef BENCHMARKING_OMP
  this->total_burning_size = 0;
#endif
}

FireStats get_fire_stats(const Fire& fire, const Landscape& landscape) {

  FireStats stats = { 0, 0, 0, 0 };

  for (std::pair<uint, uint> cell_xy : fire.burned_ids) {
    uint x = cell_xy.first;
    uint y = cell_xy.second;

    Cell cell = landscape[x, y];

    if (cell.vegetation_type == SUBALPINE) {
      stats.counts_veg_subalpine++;
    } else if (cell.vegetation_type == WET) {
      stats.counts_veg_wet++;
    } else if (cell.vegetation_type == DRY) {
      stats.counts_veg_dry++;
    } else {
      stats.counts_veg_matorral++;
    }
  }

  return stats;
}