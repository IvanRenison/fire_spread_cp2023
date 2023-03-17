#include "landscape.hpp"


#include <cstddef>

Landscape::Landscape (uint width, uint height) {
    this->width = width;
    this->height = height;
    cells = new Cell[width * height];
}

Landscape::Landscape (/* TODO: Add file parameters */) {
    // TODO: read from files
    // There is one file with de metadata, from where width and height have to ve read
    // and another file with the data for each pixel
}

Cell* Landscape::operator[](size_t index1, size_t index2) const {
    return &cells[index1 * width + index2];
}
