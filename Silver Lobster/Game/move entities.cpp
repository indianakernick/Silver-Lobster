//
//  move entities.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "move entities.hpp"

#include "tags.hpp"
#include "world.hpp"
#include "intents.hpp"
#include "position.hpp"
#include "dir to point.hpp"
#include <entt/entity/registry.hpp>

void moveEntities(entt::registry &reg) {
  const gfx::Surface<const Tile> tiles = reg.ctx<World>().tiles;
  reg.view<Position, Move>().each([&](entt::entity e, auto &pos, auto move) {
    const gfx::Point newPos = pos.p + toPoint(move.dir);
    if (pos.p == newPos) return;
    if (!tiles.contains(newPos)) return;
    switch (tiles.ref(newPos)) {
      case Tile::wall:
      case Tile::closed_door:
        return;
      case Tile::path:
      case Tile::room:
      case Tile::open_door:
      case Tile::stairs:
        pos.p = newPos;
        break;
    }
    if (reg.has<Player>(e)) {
      reg.assign<UpdateLight>(e);
    }
  });
  reg.reset<Move>();
}
