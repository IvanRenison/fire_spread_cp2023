#include "fires.hpp"

#include "landscape.hpp"
#include "matrix.hpp"

Fire read_fire(uint width, uint height, std::string filename_prefix) {

  std::ifstream burned_ids_file(filename_prefix + "-burned_ids.csv");
  CSVIterator loop(burned_ids_file);
  loop++;

  Matrix<bool> burned_layer(width, height);

  std::vector<std::pair<uint, uint>> burned_ids;
  burned_ids.reserve(width * height);

  for (; loop != CSVIterator(); ++loop) {
    uint x = atoi((*loop)[0].data());
    uint y = atoi((*loop)[1].data());
    burned_layer[x, y] = true;
    burned_ids.push_back({ x, y });
  }
  return { width, height, burned_layer, burned_ids };
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