#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include <cstddef>

template <typename T> class Matrix {
public:
  size_t width;
  size_t height;

  Matrix(size_t width, size_t height) : width(width), height(height), elems(width * height){};

  T operator[](std::pair<size_t, size_t> indexes) const {
    auto [index1, index2] = indexes;
    return elems[index2 * width + index1];
  };

  T& operator[](std::pair<size_t, size_t> indexes) {
    auto [index1, index2] = indexes;
    return elems[index2 * width + index1];
  };

  bool operator==(const Matrix& other) const {
    if (width != other.width || height != other.height) {
      return false;
    }

    for (size_t i = 0; i < width; i++) {
      for (size_t j = 0; j < height; j++) {
        if (elems[j * width + i] != other.elems[j * width + i]) {
          return false;
        }
      }
    }

    return true;
  };

  std::vector<T> elems;
};

#endif