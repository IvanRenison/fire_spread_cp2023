#ifndef _FIRES_HPP
#define _FIRES_HPP

#include <cstddef>
#include <vector>

#include "types.h"

#include "landscape.hpp"
#include "matrix.hpp"

class Fire {
public:
  uint width;
  uint height;

  Matrix<bool> burned_layer;

  std::vector<std::pair<uint, uint>> burned_ids;

  bool operator==(const Fire& other) const {
    return width == other.width && height == other.height &&
           burned_layer == other.burned_layer && burned_ids == other.burned_ids;
  }

  Fire(
      uint width, uint height, Matrix<bool> burned_layer,
      std::vector<std::pair<uint, uint>> burned_ids
  )
      : width(width), height(height), burned_layer(burned_layer), burned_ids(burned_ids){};
  Fire(uint width, uint height, std::string filename_prefix);
};

typedef struct _s_FireStats {
  uint counts_veg_matorral;
  uint counts_veg_subalpine;
  uint counts_veg_wet;
  uint counts_veg_dry;
} FireStats;

FireStats get_fire_stats(Fire fire, Landscape landscape);

#endif