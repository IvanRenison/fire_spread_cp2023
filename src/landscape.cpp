#include "landscape.hpp"

#include "matrix.hpp"

Landscape::Landscape(uint width, uint height)
    : width(width), height(height), cells(height, width) {}

Landscape::Landscape(std::string filename_prefix) : cells(0, 0) {
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

  cells = Matrix<Cell>(width, height);

  std::ifstream landscape_file(filename_prefix + "-landscape.csv");

  if (!landscape_file.is_open()) {
    throw std::runtime_error("Can't open landscape file");
  }

  CSVIterator loop_csv(landscape_file);
  ++loop_csv;

  for (uint i = 0; i < width; i++) {
    for (uint j = 0; j < height; j++, ++loop_csv) {
      if (loop_csv == CSVIterator() || (*loop_csv).size() < 8) {
        throw std::runtime_error("Invalid landscape file");
      }
      cells[i, j].subalpine = atoi((*loop_csv)[0].data());
      cells[i, j].wet = atoi((*loop_csv)[1].data());
      cells[i, j].dry = atoi((*loop_csv)[2].data());
      cells[i, j].fwi = atof((*loop_csv)[3].data());
      cells[i, j].aspect = atof((*loop_csv)[4].data());
      cells[i, j].wind_direction = atof((*loop_csv)[5].data());
      cells[i, j].elevation = atof((*loop_csv)[6].data());
      cells[i, j].burnable = atoi((*loop_csv)[7].data());
    }
  }
}

Cell Landscape::operator[](size_t index1, size_t index2) const {
  return cells[index2, index1];
}

Cell& Landscape::operator[](size_t index1, size_t index2) {
  return cells[index2, index1];
}

void Landscape::print_csv() const {
  std::cout << "\"subalpine\",\"wet\",\"dry\",\"fwi\",\"aspect\",\"wind\",\"elevation\",\"burnable\""
            << std::endl;
  for (uint i = 0; i < width; i++) {
    for (uint j = 0; j < height; j++) {
      std::cout << cells[i, j].subalpine << "," << cells[i, j].wet << ","
                << cells[i, j].dry << "," << cells[i, j].fwi << ","
                << cells[i, j].aspect << "," << cells[i, j].wind_direction << ","
                << cells[i, j].elevation << "," << cells[i, j].burnable
                << std::endl;
    }
  }
}
