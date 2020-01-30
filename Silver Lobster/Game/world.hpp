//
//  world.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef world_hpp
#define world_hpp

#include <vector>
#include "image.hpp"
#include <Graphics/geometry.hpp>

enum class Tile : uint8_t {
  wall,
  path,
  room,
  closed_door,
  open_door,
  stairs
};

enum class Visibility : uint8_t {
  unexplored,
  invisible,
  visible
};

using Region = uint8_t;

struct World {
  Image<Tile> tiles;
  Image<Region> regions;
  std::vector<gfx::Rect> rooms;
};

struct Sight {
  Image<Visibility> visibility;
};

struct Light {
  Image<bool> lit;
};

#endif
