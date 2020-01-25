//
//  world.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef world_hpp
#define world_hpp

#include <memory>
#include <Graphics/surface.hpp>

enum class Tile : uint8_t {
  wall, // 306, 0
  path, // 0, 0 or 17, 0
  room, // 0, 0
  closed_door, // 68, 153
  open_door, // 102, 153
  stairs // 34, 102
};

using Region = uint8_t;

struct World {
  std::unique_ptr<Tile[]> tileStorage;
  std::unique_ptr<Region[]> regionStorage;
  gfx::Surface<Tile> tiles;
  gfx::Surface<Region> regions;
};

#endif
