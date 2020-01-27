//
//  dir.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef dir_hpp
#define dir_hpp

#include <cstdint>
#include <cassert>

enum class Dir : uint8_t {
  up,
  up_right,
  right,
  down_right,
  down,
  down_left,
  left,
  up_left,
  none
};

constexpr Dir cardinal_dirs[] = {
  Dir::up, Dir::right, Dir::down, Dir::left
};

constexpr Dir intercardinal_dirs[] = {
  Dir::up_right, Dir::down_right, Dir::down_left, Dir::up_left
};

constexpr Dir all_dirs[] = {
  Dir::up,
  Dir::up_right,
  Dir::right,
  Dir::down_right,
  Dir::down,
  Dir::down_left,
  Dir::left,
  Dir::up_left
};

constexpr bool isCardinal(const Dir dir) noexcept {
  return dir != Dir::none && (static_cast<uint8_t>(dir) & 1) == 0;
}

constexpr Dir fromIndex(const uint8_t i) noexcept {
  return static_cast<Dir>(i & 0b111);
}

constexpr Dir fromCardinalIndex(const uint8_t i) noexcept {
  return static_cast<Dir>((i & 0b11) << 1);
}

constexpr uint8_t toIndex(const Dir d) noexcept {
  assert(d != Dir::none);
  return static_cast<uint8_t>(d);
}

constexpr uint8_t toCardinalIndex(const Dir d) noexcept {
  assert(d != Dir::none);
  assert(isCardinal(d));
  return static_cast<uint8_t>(d) >> 1;
}

constexpr Dir rotateCW90(const Dir dir) noexcept {
  if (dir == Dir::none) {
    return Dir::none;
  } else {
    return static_cast<Dir>((static_cast<uint8_t>(dir) + 2) & 0b111);
  }
}

constexpr Dir rotateCW45(const Dir dir) noexcept {
  if (dir == Dir::none) {
    return Dir::none;
  } else {
    return static_cast<Dir>((static_cast<uint8_t>(dir) + 1) & 0b111);
  }
}

constexpr Dir rotateCCW90(const Dir dir) noexcept {
  if (dir == Dir::none) {
    return Dir::none;
  } else {
    return static_cast<Dir>((static_cast<uint8_t>(dir) + 6) & 0b111);
  }
}

constexpr Dir rotateCCW45(const Dir dir) noexcept {
  if (dir == Dir::none) {
    return Dir::none;
  } else {
    return static_cast<Dir>((static_cast<uint8_t>(dir) + 7) & 0b111);
  }
}

constexpr Dir opposite(const Dir dir) noexcept {
  if (dir == Dir::none) {
    return Dir::none;
  } else {
    return static_cast<Dir>(static_cast<uint8_t>(dir) ^ 0b100);
  }
}

#endif
