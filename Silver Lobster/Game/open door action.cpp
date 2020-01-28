//
//  open door action.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 28/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "open door action.hpp"

#include "dir.hpp"
#include "world.hpp"
#include "position.hpp"
#include "dir to point.hpp"
#include <entt/entity/registry.hpp>

OpenDoorAction::OpenDoorAction()
  : dir{Dir::none} {}

OpenDoorAction::OpenDoorAction(const Dir dir)
  : dir{dir} {}

namespace {

bool open(const gfx::Surface<Tile> tiles, const gfx::Point pos) {
  if (!tiles.contains(pos)) return false;
  Tile &tile = tiles.ref(pos);
  if (tile == Tile::closed_door) {
    tile = Tile::open_door;
    return true;
  }
  return false;
}

}

Outcome OpenDoorAction::apply(entt::registry &reg, entt::entity e) {
  const gfx::Point pos = reg.get<Position>(e).p;
  const gfx::Surface<Tile> tiles = reg.ctx<World>().tiles;
  if (dir == Dir::none) {
    for (const Dir d : all_dirs) {
      if (open(tiles, pos + toPoint(d))) return true;
    }
    return false;
  } else {
    return open(tiles, pos + toPoint(dir));
  }
}
