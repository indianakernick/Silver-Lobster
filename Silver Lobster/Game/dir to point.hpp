//
//  dir to point.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef dir_to_point_hpp
#define dir_to_point_hpp

#include "dir.hpp"
#include <Graphics/geometry.hpp>

constexpr gfx::Point toPoint(const Dir dir) {
  switch (dir) {
    case Dir::up:         return {0, -1};
    case Dir::up_right:   return {1, -1};
    case Dir::right:      return {1, 0};
    case Dir::down_right: return {1, 1};
    case Dir::down:       return {0, 1};
    case Dir::down_left:  return {-1, 1};
    case Dir::left:       return {-1, 0};
    case Dir::up_left:    return {-1, -1};
    case Dir::none:       return {0, 0};
  }
}

constexpr Dir fromPoint(const gfx::Point vec) {
  if (vec == gfx::Point{0, -1}) {
    return Dir::up;
  } else if (vec == gfx::Point{1, -1}) {
    return Dir::up_right;
  } else if (vec == gfx::Point{1, 0}) {
    return Dir::right;
  } else if (vec == gfx::Point{1, 1}) {
    return Dir::down_right;
  } else if (vec == gfx::Point{0, 1}) {
    return Dir::down;
  } else if (vec == gfx::Point{-1, 1}) {
    return Dir::down_left;
  } else if (vec == gfx::Point{-1, 0}) {
    return Dir::left;
  } else if (vec == gfx::Point{-1, -1}) {
    return Dir::up_left;
  } else {
    return Dir::none;
  }
}

#endif
