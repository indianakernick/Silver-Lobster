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

#endif
