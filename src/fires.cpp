#include "fires.hpp"

#include "landscape.hpp"
#include "matrix.hpp"

FireStats get_fire_stats(Fire fire, Landscape landscape) {

  FireStats stats = {0, 0, 0, 0};

  for (std::pair<uint, uint> cell_xy : fire.burned_ids) {
    uint x = cell_xy.first;
    uint y = cell_xy.second;

    Cell* cell = landscape[x, y];

    if (cell->subalpine) {
      stats.counts_veg_subalpine++;
    }
    else if (cell->wet) {
      stats.counts_veg_wet++;
    }
    else if (cell->dry) {
      stats.counts_veg_dry++;
    }
    else {
      stats.counts_veg_matorral++;
    }
  }

  return stats;

}