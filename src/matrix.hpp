#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include <cassert>
#include <cstddef>

static_assert(sizeof(ushort) == 2, "unsigned short must be 2 bytes");
static_assert(sizeof(uint) == 4, "unsigned int must be 4 bytes");

template <typename T> class Matrix {
public:
  ushort width;
  ushort height;

  Matrix(ushort width, ushort height) : width(width), height(height) {
    uint sizei_width = 32 - __builtin_clz(width);
    uint sizei_height = 32 - __builtin_clz(height);
    uint sizei = std::max(sizei_width, sizei_height);
    uint size = (1 << sizei)*(1 << sizei);

    elems = std::vector<T>(size);
  };

  T operator[](ushort index1, ushort index2) const {
    return elems[position(index1, index2)];
  };

  T& operator[](ushort index1, ushort index2) {
    return elems[position(index1, index2)];
  };

  bool operator==(const Matrix& other) const {
    if (width != other.width || height != other.height) {
      return false;
    }

    for (ushort i = 0; i < width; i++) {
      for (ushort j = 0; j < height; j++) {
        if (elems[position(i, j)] != other.elems[position(i, j)]) {
          return false;
        }
      }
    }

    return true;
  };

  std::vector<T> elems;

private:
  uint position(ushort index1, ushort index2) const {
    // interleave the bits of index1 and index2

    uint index = 0;

    for (uint i = 0; i < 16; i++) {
      index |= ((index1 >> i) & 1) << (2 * i);
      index |= ((index2 >> i) & 1) << (2 * i + 1);
    }

    assert(index < elems.size());

    return index;
  };
};

#endif