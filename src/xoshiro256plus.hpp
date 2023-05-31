#ifndef _XOSHIRO256plus_HPP
#define _XOSHIRO256plus_HPP

#include <stdint.h>

#include "types.h"

// You can play with this value on your architecture
#define XOSHIRO256_UNROLL (8)

/* For generating seeds for Xoshiro256plus */
class splitmix64 {
public:
  uint64_t x;

  splitmix64(uint64_t seed);

  uint64_t next_splitmix64();
};

class Xoshiro256plus {
public:
  Xoshiro256plus(splitmix64& sm64);
  bool bernoulli(float p);

  void next(uint64_t* const __restrict__ array);

private:
  uint64_t s[4][XOSHIRO256_UNROLL];
  uint16_t s16[4 * XOSHIRO256_UNROLL];
  uint index = 0;

  uint64_t next_splitmix64();
};

#endif
