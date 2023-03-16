#ifndef _LANDSCAPE_HPP
#define _LANDSCAPE_HPP

#include <cstddef>
#include <string>

#include "types.h"

#define NDATA 5

typedef struct _s_Cell {
  double elevation;
  double wind_direction;
  bool burnable;
  double data[NDATA];
} Cell;

class Landscape {
public:
  uint width;
  uint height;

  Landscape(uint width, uint height);
  Landscape(uint width, uint height, std::string filename);

  Cell* operator[](size_t index1, size_t index2) const;

private:
  Cell* cells;
};


#endif