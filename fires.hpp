#ifndef _FIRES_HPP
#define _FIRES_HPP

#include <cstddef>
#include <vector>

#include "types.h"

typedef struct _s_Fire {
  uint width;
  uint height;

  bool* burned_layer;

  std::vector<std::pair<uint, uint>> burned_ids;
} Fire;


#endif