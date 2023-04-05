#ifndef _LANDSCAPE_HPP
#define _LANDSCAPE_HPP

#include <cstddef>
#include <string>

#include "csv.hpp"
#include "matrix.hpp"

#include "types.h"

typedef struct _s_Cell {
  double elevation;
  double wind_direction;
  bool burnable;
  // There are 4 types of vegetation: matorral, subalpine, wet, dry
  // bools subalpine, wet and dry are exclusive, and when the three are false,
  // the vegetation is matorral
  bool subalpine;
  bool wet;
  bool dry;
  double fwi;
  double aspect;
} Cell;

class Landscape {
public:
  uint width;
  uint height;

  Landscape(uint width, uint height);
  Landscape(std::string filename_prefix);

  Cell operator[](size_t index1, size_t index2) const;
  Cell& operator[](size_t index1, size_t index2);

private:
  Matrix<Cell> cells;
};

#endif