//
//  move action.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "move action.hpp"

#include "tags.hpp"
#include "world.hpp"
#include "combat.hpp"
#include "position.hpp"
#include "dir point.hpp"
#include "rest action.hpp"
#include "attack action.hpp"
#include "open door action.hpp"
#include <entt/entity/registry.hpp>

MoveAction::MoveAction(const Dir dir)
  : dir{dir} {}

namespace {

// Maybe a file with a set of functions dedicated to finding entities
// like finding the player, finding actors, finding items on the ground

entt::entity findTarget(entt::registry &reg, const gfx::Point pos) {
  auto view = reg.view<Position, Health>();
  for (entt::entity e : view) {
    if (view.get<Position>(e).p == pos) {
      return e;
    }
  }
  return entt::null;
}

}

Outcome MoveAction::apply(entt::registry &reg, const entt::entity e) const {
  if (dir == Dir::none) return std::make_unique<RestAction>();
  const gfx::Surface<const Tile> tiles = reg.ctx<World>().tiles;
  gfx::Point &pos = reg.get<Position>(e).p;
  const gfx::Point newPos = pos + toPoint(dir);
  if (!tiles.contains(newPos)) return false;
  if (entt::entity target = findTarget(reg, newPos); target != entt::null) {
    // prevent enemies hitting each other
    if (reg.has<Player>(target) || reg.has<Player>(e)) {
      return std::make_unique<AttackAction>(target);
    } else {
      // TODO: maybe account for entities in path finding
      // so enemies will crowd around the player instead of getting stuck in a line
      return std::make_unique<RestAction>();
    }
  }
  switch (tiles.ref(newPos)) {
    case Tile::closed_door:
      return std::make_unique<OpenDoorAction>(dir);
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
