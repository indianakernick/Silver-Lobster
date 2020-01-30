//
//  world spawning.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 30/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "world spawning.hpp"

#include <random>
#include "dir.hpp"
#include "world.hpp"
#include "position.hpp"
#include "dir point.hpp"
#include "../Factories/lamp.hpp"
#include "../Factories/ghost.hpp"
#include "../Factories/player.hpp"
#include <entt/entity/registry.hpp>

namespace {

struct SpawnParams {
  uint64_t seed;
  int brightness;  // chance of lights appearing
  int lightDanger; // chance of enemies appearing in a lit room
  int darkDanger;  // chance of enemies appearing in an unlit room
  int enemyMin;    // minimum number of enemies in a dangerous room
  int enemyMax;    // maximum number of enemies in a dangerous room
};

using IntDist = std::uniform_int_distribution<int>;

gfx::Point cornerPos(const gfx::Rect rect, const Dir corner) {
  switch (corner) {
    case Dir::up_right:   return {rect.p.x + rect.s.w - 1, rect.p.y};
    case Dir::down_right: return rect.p + rect.s.point();
    case Dir::down_left:  return {rect.p.x, rect.p.y + rect.s.h - 1};
    case Dir::up_left:    return rect.p;
    default: assert(false);
  }
}

bool allowedNearLight(const Tile tile) {
  switch (tile) {
    case Tile::wall:
    case Tile::room:
      return true;
    case Tile::open_door:
    case Tile::closed_door:
    case Tile::path:
    case Tile::stairs:
      return false;
  }
}

bool canPlaceLight(const gfx::Surface<const Tile> tiles, const gfx::Point pos) {
  if (!tiles.contains(pos)) return false;
  if (tiles.ref(pos) != Tile::room) return false;
  for (const Dir side : cardinal_dirs) {
    const gfx::Point sidePos = pos + toPoint(side);
    if (!tiles.contains(sidePos)) continue;
    if (!allowedNearLight(tiles.ref(sidePos))) return false;
  }
  return true;
}

bool entityAt(entt::registry &reg, const gfx::Point pos) {
  // TODO: Speed this up
  // This operation is going to be used for a lot of things
  // Maybe a std::unordered_map<gfx::Point, ???>
  auto view = reg.view<Position>();
  const Position *begin = view.raw();
  const Position *end = begin + view.size();
  for (const Position *p = begin; p != end; ++p) {
    if (p->p == pos) return true;
  }
  return false;
}

}

// TODO: factories for making categories of things
// They'd randomly choose which concrete entity to create
// makeLight() might choose between a torch, chandelier, etc
// the chosen entity might depend on level
// harder enemies spawner later in the game

void spawnEntities(entt::registry &reg) {
  // TODO: Generate these params from a level number
  const SpawnParams params = {
    .seed = 12345,
    .brightness = 90,
    .lightDanger = 20,
    .darkDanger = 30,
    .enemyMin = 1,
    .enemyMax = 3
  };
  
  const World &world = reg.ctx<World>();
  const gfx::Rect playerRoom = world.rooms[1];
  makePlayer(reg, playerRoom.p + playerRoom.s.point() / 2);
  
  gfx::Surface<const Tile> tiles = world.tiles;
  std::mt19937_64 rng{params.seed};
  IntDist chanceDist{0, 99};
  IntDist enemyDist{params.enemyMin, params.enemyMax};
  
  for (gfx::Rect room : world.rooms) {
    bool lit = chanceDist(rng) < params.brightness;
    if (lit) {
      Dir corners[4];
      std::memcpy(corners, intercardinal_dirs, sizeof(corners));
      std::shuffle(corners, corners + 4, rng);
      bool placed = false;
      for (const Dir corner : corners) {
        const gfx::Point pos = cornerPos(room, corner);
        if (canPlaceLight(tiles, pos) && !entityAt(reg, pos)) {
          makeLamp(reg, {pos});
          placed = true;
          break;
        }
      }
      lit = placed;
    }
    
    if (chanceDist(rng) < (lit ? params.lightDanger : params.darkDanger)) {
      IntDist xDist{0, room.s.w - 1};
      IntDist yDist(0, room.s.h - 1);
      int enemies = enemyDist(rng);
      while (enemies--) {
        while (true) {
          const gfx::Point pos = {room.p.x + xDist(rng), room.p.y + yDist(rng)};
          if (entityAt(reg, pos)) continue;
          makeGhost(reg, pos);
          break;
        }
      }
    }
  }
}
