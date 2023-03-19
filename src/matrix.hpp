#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include <cstddef>

template <typename T> class Matrix {
public:
  size_t width;
  size_t height;

  Matrix(size_t width, size_t height) {
    this->width = width;
    this->height = height;
    elems = new T[width * height];
  };

  T* operator[](size_t index1, size_t index2) const { return &elems[index1 * width + index2] };

private:
  T* elems;
};

#endif