#ifndef _LANDSCAPE_HPP
#define _LANDSCAPE_HPP

#include <cstddef>
#include <string>

#include "types.h"
#include "csv.hpp"

#define N_BOOLEAN_DATA 3
#define N_FLOATING_DATA 2

typedef struct _s_Cell {
  double elevation;
  double wind_direction;
  bool burnable;
  bool boolean_data[N_BOOLEAN_DATA];
  double floating_data[N_FLOATING_DATA];
} Cell;

class Landscape {
public:
  uint width;
  uint height;

  Landscape(uint width, uint height);
  Landscape(std::string filename_prefix);

  Cell* operator[](size_t index1, size_t index2) const;

private:
  Cell* cells;
};


#endif