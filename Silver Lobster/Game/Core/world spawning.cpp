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
  int brightness;    // chance of lights appearing
  int lightDanger;   // chance of enemies appearing in a lit room
  int darkDanger;    // chance of enemies appearing in an unlit room
  int enemyMin;      // minimum number of enemies in a dangerous room
  int enemyMax;      // maximum number of enemies in a dangerous room
  int spawnAttempts; // maximum number of attempts to spawn enemies in a room
};

using IntDist = std::uniform_int_distribution<int>;

class Spawner {
public:
  Spawner(entt::registry &reg, World &world)
    : reg{reg}, tiles{world.tiles}, rooms{world.rooms} {}
  
  void spawn(const SpawnParams &params) {
    rng.seed(params.seed);
    spawnPlayer();
    for (const gfx::Rect room : rooms) {
      if (dangerous(placeLight(room, params), params)) {
        spawnEnemies(room, params);
      }
    }
  }
  
private:
  entt::registry &reg;
  gfx::Surface<const Tile> tiles;
  const std::vector<gfx::Rect> &rooms;
  std::mt19937_64 rng;
  
  static gfx::Point cornerPos(const gfx::Rect rect, const Dir corner) {
    switch (corner) {
      case Dir::up_right:   return {rect.p.x + rect.s.w - 1, rect.p.y};
      case Dir::down_right: return rect.p + rect.s.point();
      case Dir::down_left:  return {rect.p.x, rect.p.y + rect.s.h - 1};
      case Dir::up_left:    return rect.p;
      default: assert(false);
    }
  }
  
  static bool validLightSourceNeighbor(const Tile tile) {
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
  
  bool validLightSource(const gfx::Point pos) {
    if (!tiles.contains(pos)) return false;
    if (tiles.ref(pos) != Tile::room) return false;
    for (const Dir side : cardinal_dirs) {
      const gfx::Point sidePos = pos + toPoint(side);
      if (!tiles.contains(sidePos)) continue;
      if (!validLightSourceNeighbor(tiles.ref(sidePos))) return false;
    }
    return true;
  }
  
  bool occupied(const gfx::Point pos) {
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
  
  void spawnPlayer() {
    // exit is in room[0]
    // player is in room[1]
    makePlayer(reg, rooms[1].p + rooms[1].s.point() / 2);
  }
  
  bool placeLight(const gfx::Rect room, const SpawnParams &params) {
    assert(0 <= params.brightness && params.brightness <= 100);
    
    IntDist lightDist{0, 99};
    if (lightDist(rng) >= params.brightness) return false;
    Dir corners[4];
    std::copy(intercardinal_dirs, intercardinal_dirs + 4, corners);
    std::shuffle(corners, corners + 4, rng);
    bool placed = false;
    for (const Dir corner : corners) {
      const gfx::Point pos = cornerPos(room, corner);
      if (validLightSource(pos) && !occupied(pos)) {
        makeLamp(reg, {pos});
        placed = true;
        break;
      }
    }
    return placed;
  }
  
  bool dangerous(const bool lit, const SpawnParams &params) {
    assert(0 <= params.lightDanger && params.lightDanger <= 100);
    assert(0 <= params.darkDanger && params.darkDanger <= 100);
    
    IntDist dangerDist{0, 99};
    return dangerDist(rng) < (lit ? params.lightDanger : params.darkDanger);
  }
  
  void spawnEnemies(const gfx::Rect room, const SpawnParams &params) {
    assert(params.enemyMin >= 0);
    assert(params.enemyMax > 0);
    assert(params.enemyMin <= params.enemyMax);
    assert(params.spawnAttempts > 0);
    
    IntDist enemyDist{params.enemyMin, params.enemyMax};
    IntDist xDist{room.p.x, room.p.x + room.s.w - 1};
    IntDist yDist{room.p.y, room.p.y + room.s.h - 1};
    int enemies = enemyDist(rng);
    while (enemies--) {
      int attempts = params.spawnAttempts;
      while (attempts--) {
        const gfx::Point pos = {xDist(rng), yDist(rng)};
        if (!occupied(pos)) {
          makeGhost(reg, pos);
          break;
        }
      }
    }
  }
};

}

// TODO: factories for making categories of things
// They'd randomly choose which concrete entity to create
// makeLight() might choose between a torch, chandelier, etc
// the chosen entity might depend on level
// harder enemies spawner later in the game

void spawnEntities(entt::registry &reg, const uint64_t seed) {
  // TODO: Generate these params from a level number
  const SpawnParams params = {
    .seed = seed,
    .brightness = 90,
    .lightDanger = 20,
    .darkDanger = 30,
    .enemyMin = 1,
    .enemyMax = 3,
    .spawnAttempts = 10
  };
  
  Spawner spawner{reg, reg.ctx<World>()};
  spawner.spawn(params);
}
