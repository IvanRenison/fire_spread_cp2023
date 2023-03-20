#ifndef _IGNITIONS_CELLS_HPP
#define _IGNITIONS_CELLS_HPP

#include <string>
#include <vector>

#include "types.h"

typedef std::vector<std::pair<uint, uint>> IgnitionCells;

IgnitionCells read_ignition_cells(std::string filename);

#endif