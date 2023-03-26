#include "landscape.hpp"

Landscape::Landscape(uint width, uint height) {
  this->width = width;
  this->height = height;
  cells = new Cell[width * height];
}

Landscape::Landscape(std::string filename_prefix) {
  std::ifstream metadata_file(filename_prefix + "-metadata.csv");

  if (!metadata_file.is_open()) {
    throw std::runtime_error("Can't open metadata file");
  }

  CSVIterator metadata_csv(metadata_file);
  ++metadata_csv;
  if (metadata_csv == CSVIterator() || (*metadata_csv).size() < 2) {
    throw std::runtime_error("Invalid metadata file");
  }

  width = atoi((*metadata_csv)[0].data());
  height = atoi((*metadata_csv)[1].data());

  cells = new Cell[height * width];

  std::ifstream landscape_file(filename_prefix + "-landscape.csv");

  if (!landscape_file.is_open()) {
    throw std::runtime_error("Can't open landscape file");
  }

  CSVIterator loop_csv(landscape_file);
  ++loop_csv;

  for (uint i = 0; i < width * height; i++, ++loop_csv) {
    if (loop_csv == CSVIterator() || (*loop_csv).size() < 8) {
      throw std::runtime_error("Invalid landscape file");
    }
    cells[i].subalpine = atoi((*loop_csv)[0].data());
    cells[i].wet = atoi((*loop_csv)[1].data());
    cells[i].dry = atoi((*loop_csv)[2].data());
    cells[i].fwi = atof((*loop_csv)[3].data());
    cells[i].aspect = atof((*loop_csv)[4].data());
    cells[i].wind_direction = atof((*loop_csv)[5].data());
    cells[i].elevation = atof((*loop_csv)[6].data());
    cells[i].burnable = atoi((*loop_csv)[7].data());
  }
}

Cell* Landscape::operator[](size_t index1, size_t index2) const {
  return &cells[index2 * width + index1];
}
