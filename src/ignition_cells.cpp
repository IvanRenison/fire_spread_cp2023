#include "ignition_cells.hpp"

#include <string>
#include <vector>

#include "types.h"

#include "csv.hpp"

IgnitionCells read_ignition_cells(std::string filename) {

  std::ifstream file(filename);

  CSVIterator loop(file);
  loop++; // skip first line

  IgnitionCells ignition_cells;

  for (; loop != CSVIterator(); ++loop) {
    ignition_cells.push_back({ atoi((*loop)[0].data()), atoi((*loop)[1].data()) });
  }

  return ignition_cells;
}
