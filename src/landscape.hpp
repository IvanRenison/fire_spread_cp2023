#ifndef _LANDSCAPE_HPP
#define _LANDSCAPE_HPP

#include <cassert>
#include <cstddef>
#include <string>

#include "csv.hpp"
#include "matrix.hpp"

#include "types.h"

// enum of vegetation type between: matorral, subalpine, wet, dry
enum VegetationType { MATORRAL, SUBALPINE, WET, DRY } __attribute__((packed));

static_assert(sizeof(VegetationType) == 1);

typedef struct _s_Cell {
  float elevation;
  float wind_direction;
  bool burnable;
  VegetationType vegetation_type;
  float fwi;
  float aspect;
} Cell;

class Landscape {
public:
  uint width;
  uint height;

  Landscape(uint width, uint height);
  Landscape(std::string filename_prefix);

  Cell operator[](size_t index1, size_t index2) const;
  Cell& operator[](size_t index1, size_t index2);

  void print_csv() const;

private:
  Matrix<Cell> cells;
};

#endif