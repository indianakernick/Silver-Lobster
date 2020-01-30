//
//  world generation.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "world generation.hpp"

#include <set>
#include <random>
#include "world.hpp"
#include "dir set.hpp"
#include <unordered_set>
#include <unordered_map>
#include "dir point.hpp"
#include <Graphics/fill.hpp>
#include <Graphics/compare.hpp>
#include <entt/entity/registry.hpp>

// Dungeon generation algorithm by Bob Nystrom
// http://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/

namespace {

struct GenParams {
  uint64_t seed;
  int roomSizeMin;
  int roomSizeMax;
  int roomDensity;
  int mazeStraightness;
  int connectionRedundancy;
};

constexpr Region null_region = ~Region{};

using IntDist = std::uniform_int_distribution<int>;

gfx::Rect addMargin(const gfx::Rect rect) {
  return {rect.p - 1, rect.s + 2};
}

class Generator {
public:
  explicit Generator(World &world)
    : tiles{world.tiles}, regions{world.regions} {}

  void generate(const GenParams &params) {
    carve(tiles.rect(), Tile::wall);
    rng.seed(params.seed);
    placeRooms(params);
    growMazes(params);
    connectRegions(params);
    removeDeadEnds();
  }

private:
  gfx::Surface<Tile> tiles;
  gfx::Surface<Region> regions;
  std::mt19937_64 rng;
  Region currentRegion = null_region;
  
  void startRegion() {
    static bool started = false;
    ++currentRegion;
    assert(!started || currentRegion != 0);
    started = true;
  }
  
  void carve(const gfx::Point pos, const Tile tile) {
    tiles.ref(pos) = tile;
    regions.ref(pos) = currentRegion;
  }
  
  void carve(const gfx::Rect rect, const Tile tile) {
    gfx::fill(tiles.view(rect), tile);
    gfx::fill(regions.view(rect), currentRegion);
  }
  
  // ------------------------------- rooms ---------------------------------- //
  
  void placeRooms(const GenParams &params) {
    assert(params.roomSizeMin > 0 && params.roomSizeMin % 2 == 1);
    assert(params.roomSizeMax > params.roomSizeMin && params.roomSizeMax % 2 == 1);
    assert(params.roomSizeMin <= tiles.width() - 2);
    assert(params.roomSizeMin <= tiles.height() - 2);
    assert(params.roomDensity > 0);

    IntDist sizeDist{params.roomSizeMin / 2, params.roomSizeMax / 2};
    for (int d = params.roomDensity; d--; ) {
      const gfx::Size size{sizeDist(rng) * 2 + 1, sizeDist(rng) * 2 + 1};
      IntDist xDist{0, (tiles.width() - size.w) / 2 - 1};
      IntDist yDist{0, (tiles.height() - size.h) / 2 - 1};
      const gfx::Point pos{xDist(rng) * 2 + 1, yDist(rng) * 2 + 1};
      const gfx::Rect rect{pos, size};

      if (gfx::equal(tiles.view(addMargin(rect)), Tile::wall)) {
        startRegion();
        carve(rect, Tile::room);
      }
    }
  }
  
  // ------------------------------- mazes ---------------------------------- //
  
  Dir randomDir(const DirSet dirs) {
    assert(dirs.anyCardinal());
    std::uniform_int_distribution<uint8_t> dirDist{0, 3};
    Dir dir = fromCardinalIndex(dirDist(rng));
    while (!dirs.test(dir)) {
      dir = rotateCW90(dir);
    }
    return dir;
  }
  
  bool canCarve(const gfx::Point pos, const Dir dir) {
    if (!tiles.contains(pos + toPoint(dir) * 3)) return false;
    return tiles.ref(pos + toPoint(dir) * 2) == Tile::wall;
  }
  
  void growMaze(const gfx::Point start, const GenParams &params) {
    assert(0 <= params.mazeStraightness && params.mazeStraightness <= 100);
    
    IntDist straightDist{0, 99};
    std::vector<gfx::Point> cells;
    Dir lastDir = Dir::none;
    
    startRegion();
    carve(start, Tile::path);
    cells.push_back(start);
    
    while (!cells.empty()) {
      gfx::Point cell = cells.back();
      DirSet unmadeCells;

      for (const Dir dir : cardinal_dirs) {
        if (canCarve(cell, dir)) {
          unmadeCells.set(dir);
        }
      }

      if (unmadeCells.any()) {
        Dir dir = randomDir(unmadeCells);
        if (lastDir != Dir::none) {
          if (unmadeCells.test(lastDir)) {
            if (straightDist(rng) < params.mazeStraightness) {
              dir = lastDir;
            }
          }
        }

        carve(cell + toPoint(dir), Tile::path);
        carve(cell + toPoint(dir) * 2, Tile::path);

        cells.push_back(cell + toPoint(dir) * 2);
        lastDir = dir;
      } else {
        cells.pop_back();
        lastDir = Dir::none;
      }
    }
  }
  
  void growMazes(const GenParams &params) {
    for (int y = 1; y != tiles.height(); y += 2) {
      for (int x = 1; x != tiles.width(); x += 2) {
        if (tiles.ref(x, y) == Tile::wall) {
          growMaze({x, y}, params);
        }
      }
    }
  }
  
  // ---------------------------- connections ------------------------------- //
  
  using RegionSet = std::set<Region>;
  using ConnectorMap = std::unordered_map<gfx::Point, RegionSet>;
  using RegionMap = std::unordered_map<Region, Region>;
  using BigRegionSet = std::unordered_set<Region>;
  
  ConnectorMap findConnectors() {
    ConnectorMap connectors;
    for (int y = 1; y != tiles.height() - 1; ++y) {
      for (int x = 1; x != tiles.width() - 1; ++x) {
        if (tiles.ref(x, y) != Tile::wall) continue;
        RegionSet connectorRegions;
        for (const Dir dir : cardinal_dirs) {
          const Region region = regions.ref(gfx::Point{x, y} + toPoint(dir));
          if (region != null_region) {
            connectorRegions.insert(region);
          }
        }
        if (connectorRegions.size() >= 2) {
          connectors.insert({{x, y}, connectorRegions});
        }
      }
    }
    return connectors;
  }
  
  static RegionSet applyMapping(const RegionMap &mapping, const RegionSet &regions) {
    RegionSet merged;
    for (const Region region : regions) {
      merged.insert(mapping.at(region));
    }
    return merged;
  }
  
  static bool close(const gfx::Point a, const gfx::Point b) {
    return std::abs(a.x - b.x) < 2 && std::abs(a.y - b.y) < 2;
  }
  
  void connectRegions(const GenParams &params) {
    assert(0 <= params.connectionRedundancy && params.connectionRedundancy <= 100);

    ConnectorMap connectors = findConnectors();
    RegionMap mergeMapping;
    BigRegionSet openRegions;
    IntDist redundancyDist{0, 99};

    for (Region r = 0; r <= currentRegion; ++r) {
      mergeMapping.insert({r, r});
      openRegions.insert(r);
    }

    while (openRegions.size() > 1) {
      std::uniform_int_distribution<size_t> connectorDist{0, connectors.size() - 1};
      const auto conIter = std::next(connectors.cbegin(), connectorDist(rng));
      const gfx::Point conPos = conIter->first;
      const RegionSet conRegion = conIter->second;

      tiles.ref(conPos) = Tile::closed_door;

      RegionSet sources = applyMapping(mergeMapping, conRegion);
      const Region dest = *sources.cbegin();
      sources.erase(sources.begin());

      for (Region r = 0; r <= currentRegion; ++r) {
        Region &mergedRegion = mergeMapping.at(r);
        if (sources.find(mergedRegion) != sources.end()) {
          mergedRegion = dest;
        }
      }

      for (const Region sourceRegion : sources) {
        openRegions.erase(sourceRegion);
      }

      for (auto con = connectors.cbegin(); con != connectors.cend(); ) {
        if (close(con->first, conPos)) {
          con = connectors.erase(con);
          continue;
        }

        if (applyMapping(mergeMapping, con->second).size() > 1) {
          ++con;
          continue;
        }

        if (redundancyDist(rng) < params.connectionRedundancy) {
          tiles.ref(con->first) = Tile::closed_door;
        }

        con = connectors.erase(con);
      }
    }
  }
  
  // ----------------------------- dead ends -------------------------------- //
  
  void removeDeadEnds() {
    bool done = false;

    while (!done) {
      done = true;

      for (int y = 1; y != tiles.height() - 1; ++y) {
        for (int x = 1; x != tiles.width() - 1; ++x) {
          if (tiles.ref(x, y) == Tile::wall) continue;

          DirSet exits;
          for (const Dir dir : cardinal_dirs) {
            if (tiles.ref(gfx::Point{x, y} + toPoint(dir)) != Tile::wall) {
              exits.set(dir);
            }
          }
          
          if (exits.count() == 1) {
            done = false;
            tiles.ref(x, y) = Tile::wall;
          }
        }
      }
    }
  }
};

}

void initializeWorld(entt::registry &reg, const gfx::Size size) {
  assert(size.w > 1 && size.w % 2 == 1);
  assert(size.h > 1 && size.h % 2 == 1);
  World &world = reg.set<World>();
  world.tiles = {size};
  world.regions = {size};
}

void generateTerrain(entt::registry &reg) {
  const GenParams params = {
    .seed = 12345,
    .roomSizeMin = 3,
    .roomSizeMax = 9,
    .roomDensity = 200,
    .mazeStraightness = 100,
    .connectionRedundancy = 2
  };
  Generator gen{reg.ctx<World>()};
  gen.generate(params);
}
