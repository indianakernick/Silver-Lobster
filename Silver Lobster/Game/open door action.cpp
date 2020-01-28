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

Outcome OpenDoorAction::apply(entt::registry &reg, entt::entity e) {
  const gfx::Point pos = reg.get<Position>(e).p;
  const gfx::Surface<Tile> tiles = reg.ctx<World>().tiles;
  for (const Dir dir : all_dirs) {
    const gfx::Point doorPos = pos + toPoint(dir);
    if (!tiles.contains(doorPos)) continue;
    Tile &tile = tiles.ref(doorPos);
    if (tile == Tile::closed_door) {
      tile = Tile::open_door;
      return true;
    }
  }
  return false;
}
