#include "fires.hpp"

#include "landscape.hpp"
#include "matrix.hpp"

Fire::Fire(uint width, uint height, std::string filename_prefix)
    : Fire(width, height, Matrix<bool>(width, height), std::vector<std::pair<uint, uint>>()) {
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

  this->burned_layer = burned_layer;
  this->burned_ids = burned_ids;
}

FireStats get_fire_stats(Fire fire, Landscape landscape) {

  FireStats stats = { 0, 0, 0, 0 };

  for (std::pair<uint, uint> cell_xy : fire.burned_ids) {
    uint x = cell_xy.first;
    uint y = cell_xy.second;

    Cell* cell = landscape[x, y];

    if (cell->subalpine) {
      stats.counts_veg_subalpine++;
    } else if (cell->wet) {
      stats.counts_veg_wet++;
    } else if (cell->dry) {
      stats.counts_veg_dry++;
    } else {
      stats.counts_veg_matorral++;
    }
  }

  return stats;
}