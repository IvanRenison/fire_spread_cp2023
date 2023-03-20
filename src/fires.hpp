#ifndef _FIRES_HPP
#define _FIRES_HPP

#include <cstddef>
#include <vector>

#include "types.h"

#include "landscape.hpp"
#include "matrix.hpp"

typedef struct _s_Fire {
  uint width;
  uint height;

  Matrix<bool> burned_layer;

  std::vector<std::pair<uint, uint>> burned_ids;
} Fire;

typedef struct _s_FireStats {
  uint counts_veg_matorral;
  uint counts_veg_subalpine;
  uint counts_veg_wet;
  uint counts_veg_dry;
} FireStats;

FireStats get_fire_stats(Fire fire, Landscape landscape);

#endif