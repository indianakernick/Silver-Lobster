//
//  rng.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 30/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "rng.hpp"

#include <random>
#include <cassert>

// XorShift* implementation by Arvid Gerstmann
// https://arvid.io/2018/07/02/better-cxx-prng

// RNG::RNG()
//   : seed_{0xC1F651C67C62C6E0} {}

RNG::RNG(const result_type seed)
  : seed_{seed} {
  assert(seed != 0);
}

void RNG::seed(const result_type seed) {
  assert(seed != 0);
  seed_ = seed;
}

RNG::result_type RNG::operator()() {
  seed_ ^= seed_ >> 11;
  seed_ ^= seed_ << 31;
  seed_ ^= seed_ >> 18;
  return seed_ * 0xD989BCACC137DCD5;
}

int RNG::range(const int min, const int max) {
  assert(min <= max);
  return std::uniform_int_distribution<>{min, max}(*this);
}

bool RNG::percent(const int prob) {
  assert(0 <= prob && prob <= 100);
  return range(0, 99) < prob;
}

gfx::Point RNG::point(const gfx::Rect rect) {
  return rect.p + point(rect.s);
}

gfx::Point RNG::point(const gfx::Size size) {
  assert(!size.empty());
  return {range(0, size.w - 1), range(0, size.h - 1)};
}

gfx::Size RNG::size(const int min, const int max) {
  assert(min >= 0);
  return {range(min, max), range(min, max)};
}

gfx::Point RNG::oddPoint(const gfx::Size size) {
  return point(size / 2) * 2 + 1;
}

gfx::Size RNG::oddSize(const int min, const int max) {
  return size(min / 2, max / 2) * 2 + 1;
}
