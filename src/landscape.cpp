#include "landscape.hpp"

Landscape::Landscape(uint width, uint height) {
  this->width = width;
  this->height = height;
  cells = new Cell[width * height];
}

Landscape::Landscape(std::string filename_prefix) {
  uint i = 0;
  // for skiping first line with column names
  bool first_loop = true;
  std::ifstream metadata(filename_prefix + "-metadata.csv");
  for (CSVIterator loop(metadata); loop != CSVIterator(); ++loop) {
    if (first_loop) {
      first_loop = !first_loop;
      continue;
    }
    this->height = atoi((*loop)[0].data());
    this->width = atoi((*loop)[1].data());
  }

  cells = new Cell[this->height * this->width];

  std::ifstream landscape(filename_prefix + "-landscape.csv");

  i = 0;
  first_loop = true;

  for (CSVIterator loop(landscape); loop != CSVIterator(); ++loop) {
    // cells are column vercrorized
    if (first_loop) {
      first_loop = !first_loop;
      continue;
    }
    cells[i].subalpine = atoi((*loop)[0].data());
    cells[i].wet = atoi((*loop)[1].data());
    cells[i].dry = atoi((*loop)[2].data());
    cells[i].fwi = atof((*loop)[3].data());
    cells[i].aspect = atof((*loop)[4].data());
    cells[i].wind_direction = atof((*loop)[5].data());
    cells[i].elevation = atof((*loop)[6].data());
    cells[i].burnable = atoi((*loop)[7].data());
    i++;
  }
}

Cell* Landscape::operator[](size_t index1, size_t index2) const {
    return &cells[index2 * width + index1];
}
