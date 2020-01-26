//
//  world.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef world_hpp
#define world_hpp

#include "image.hpp"

enum class Tile : uint8_t {
  wall,
  path,
  room,
  closed_door,
  open_door,
  stairs
};

using Region = uint8_t;

struct World {
  Image<Tile> tiles;
  Image<Region> regions;
};

#endif
