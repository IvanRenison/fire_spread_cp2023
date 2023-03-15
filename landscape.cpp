#include "landscape.hpp"


#include <cstddef>

Landscape::Landscape (uint width, uint height) {
    this->width = width;
    this->height = height;
    cells = new Cell[width * height];
}

Landscape::Landscape (uint width, uint height, std::string filename) {
    this->width = width;
    this->height = height;
    cells = new Cell[width * height];

    // TODO: read from file
}

Cell* Landscape::operator[](size_t index1, size_t index2) const {
    return &cells[index1 * width + index2];
}
