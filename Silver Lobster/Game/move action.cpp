//
//  move action.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "move action.hpp"

#include "world.hpp"
#include "position.hpp"
#include "dir to point.hpp"
#include "open door action.hpp"
#include <entt/entity/registry.hpp>

MoveAction::MoveAction(const Dir dir)
  : dir{dir} {}

Outcome MoveAction::apply(entt::registry &reg, const entt::entity e) {
  const gfx::Surface<const Tile> tiles = reg.ctx<World>().tiles;
  gfx::Point &pos = reg.get<Position>(e).p;
  const gfx::Point newPos = pos + toPoint(dir);
  if (pos == newPos) return false;
  if (!tiles.contains(newPos)) return false;
  switch (tiles.ref(newPos)) {
    case Tile::closed_door:
      return std::make_unique<OpenDoorAction>();
    case Tile::wall:
      return false;
    case Tile::path:
    case Tile::room:
    case Tile::open_door:
    case Tile::stairs:
      pos = newPos;
      break;
  }
  return true;
}
