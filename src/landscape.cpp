#include "landscape.hpp"

Landscape::Landscape(uint width, uint height)
    : width(width), height(height), cells(height * width) {}

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

  cells = std::vector<Cell>(width * height);

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
    if (atoi((*loop_csv)[0].data()) == 1) {
      cells[i].vegetation_type = SUBALPINE;
    } else if (atoi((*loop_csv)[1].data()) == 1) {
      cells[i].vegetation_type = WET;
    } else if (atoi((*loop_csv)[2].data()) == 1) {
      cells[i].vegetation_type = DRY;
    } else {
      cells[i].vegetation_type = MATORRAL;
    }
    cells[i].fwi = atof((*loop_csv)[3].data());
    cells[i].aspect = atof((*loop_csv)[4].data());
    cells[i].wind_direction = atof((*loop_csv)[5].data());
    cells[i].elevation = atof((*loop_csv)[6].data());
    cells[i].burnable = atoi((*loop_csv)[7].data());
  }
}

Cell Landscape::operator[](size_t index1, size_t index2) const {
  return cells[index2 * width + index1];
}

Cell& Landscape::operator[](size_t index1, size_t index2) {
  return cells[index2 * width + index1];
}

void Landscape::print_csv() const {
  std::cout
      << "\"subalpine\",\"wet\",\"dry\",\"fwi\",\"aspect\",\"wind\",\"elevation\",\"burnable\""
      << std::endl;
  for (uint i = 0; i < width; i++) {
    for (uint j = 0; j < height; j++) {
      std::cout << (cells[j * width + i].vegetation_type == SUBALPINE) << ","
                << (cells[j * width + i].vegetation_type == WET) << ","
                << (cells[j * width + i].vegetation_type == DRY) << ","
                << cells[j * width + i].fwi << "," << cells[j * width + i].aspect << ","
                << cells[j * width + i].wind_direction << "," << cells[j * width + i].elevation
                << "," << cells[j * width + i].burnable << std::endl;
    }
  }
}
