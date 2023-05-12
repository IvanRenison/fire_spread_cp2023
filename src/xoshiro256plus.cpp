/*  Written in 2019 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */
#include "xoshiro256plus.hpp"

#include <stdint.h>

#define SEED 10
static uint64_t x = SEED;
static uint64_t next_splitmix64() {
	uint64_t z = (x += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

// You can play with this value on your architecture
#define XOSHIRO256_UNROLL (8)

/* The current state of the generators. */
static uint64_t s[4][XOSHIRO256_UNROLL] = {
    {next_splitmix64(), next_splitmix64(), next_splitmix64(), next_splitmix64(),
     next_splitmix64(), next_splitmix64(), next_splitmix64(),
     next_splitmix64()},
    {next_splitmix64(), next_splitmix64(), next_splitmix64(), next_splitmix64(),
     next_splitmix64(), next_splitmix64(), next_splitmix64(),
     next_splitmix64()},
    {next_splitmix64(), next_splitmix64(), next_splitmix64(), next_splitmix64(),
     next_splitmix64(), next_splitmix64(), next_splitmix64(),
     next_splitmix64()},
    {next_splitmix64(), next_splitmix64(), next_splitmix64(), next_splitmix64(),
     next_splitmix64(), next_splitmix64(), next_splitmix64(),
     next_splitmix64()}};

static __inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

uint64_t next(uint64_t * const __restrict__ array, int len) {
 	uint64_t t[XOSHIRO256_UNROLL];

	for(int b = 0; b < len; b += XOSHIRO256_UNROLL) {
		for(int i = 0; i < XOSHIRO256_UNROLL; i++) array[b + i] = s[0][i] + s[3][i];

		for(int i = 0; i < XOSHIRO256_UNROLL; i++) t[i] = s[1][i] << 17;

		for(int i = 0; i < XOSHIRO256_UNROLL; i++) s[2][i] ^= s[0][i];
		for(int i = 0; i < XOSHIRO256_UNROLL; i++) s[3][i] ^= s[1][i];
		for(int i = 0; i < XOSHIRO256_UNROLL; i++) s[1][i] ^= s[2][i];
		for(int i = 0; i < XOSHIRO256_UNROLL; i++) s[0][i] ^= s[3][i];

		for(int i = 0; i < XOSHIRO256_UNROLL; i++) s[2][i] ^= t[i];

		for(int i = 0; i < XOSHIRO256_UNROLL; i++) s[3][i] = rotl(s[3][i], 45);
	}

	// This is just to avoid dead-code elimination
	return array[0] ^ array[len - 1];
}

