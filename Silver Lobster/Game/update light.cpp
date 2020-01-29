//
//  update light.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "update light.hpp"

#include <cmath>
#include "tags.hpp"
#include "world.hpp"
#include "position.hpp"
#include "light source.hpp"
#include <Graphics/each.hpp>
#include <Graphics/fill.hpp>
#include <entt/entity/registry.hpp>

namespace {

// Vision algorithm by Adam Milazzo
// http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html

struct Slope {
  unsigned y, x;
  
  bool operator<(const Slope other) const {
    return y * other.x < x * other.y;
  }
  bool operator>(const Slope other) const {
    return y * other.x > x * other.y;
  }
  bool operator<=(const Slope other) const {
    return y * other.x <= x * other.y;
  }
  bool operator>=(const Slope other) const {
    return y * other.x >= x * other.y;
  }
};

template <typename Policy>
class Vision {
public:
  ~Vision() {
    static_assert(std::is_base_of_v<Vision, Policy>);
  }

  void update(const gfx::Point origin, const int range) {
    that()->setVisible(origin.x, origin.y);
    for (unsigned octant = 0; octant != 8; ++octant) {
      update(octant, origin, range);
    }
    // 0 right - up_right
    // 1 up_right - up
    // 2 up - up_left
    // 3 up_left - left
    // 4 left - down_left
    // 5 down_left - down
    // 6 down - down_right
    // 7 down_right - right
  }
  
  void update(const unsigned octant, const gfx::Point origin, const int range) {
    update(octant, origin, range, 1, {1, 1}, {0, 1});
  }
  
private:
  Policy *that() {
    return static_cast<Policy *>(this);
  }
  const Policy *that() const {
    return static_cast<const Policy *>(this);
  }
  
  static int getDistance(const unsigned x, const unsigned y) {
    return std::sqrt(x * x + y * y);
  }
  
  static gfx::Point octantOffset(const int x, const int y, const unsigned octant) {
    switch(octant) {
      case 0: return {x, -y};
      case 1: return {y, -x};
      case 2: return {-y, -x};
      case 3: return {-x, -y};
      case 4: return {-x, y};
      case 5: return {-y, x};
      case 6: return {y, x};
      case 7: return {x, y};
      default: return {};
    }
  }
  
  bool blocksLight(const unsigned x, const unsigned y, const unsigned octant, gfx::Point origin) const {
    origin += octantOffset(x, y, octant);
    return that()->blocksLight(origin.x, origin.y);
  }
  
  void setVisible(const unsigned x, const unsigned y, const unsigned octant, gfx::Point origin) {
    origin += octantOffset(x, y, octant);
    that()->setVisible(origin.x, origin.y);
  }
  
  void update(
    const unsigned octant,
    const gfx::Point origin,
    const int range,
    unsigned x,
    Slope top,
    Slope bottom
  ) {
    // It's an absolute mess but it works!
    for (; x <= unsigned(range); ++x) {
      unsigned topY;
      if (top.x == 1) {
        topY = x;
      } else {
        topY = ((x * 2 - 1) * top.y + top.x) / (top.x * 2);
        if (blocksLight(x, topY, octant, origin)) {
          if (top >= Slope{topY * 2 + 1, x * 2}) {
            if (!blocksLight(x, topY + 1, octant, origin)) {
              ++topY;
            }
          }
        } else {
          const unsigned ax = x * 2 + blocksLight(x + 1, topY + 1, octant, origin);
          topY += (top > Slope{topY * 2 + 1, ax});
        }
      }
      
      unsigned bottomY;
      if (bottom.y == 0) {
        bottomY = 0;
      } else {
        bottomY = ((x * 2 - 1) * bottom.y + bottom.x) / (bottom.x * 2);
        if (bottom >= Slope{bottomY * 2 + 1, x * 2}) {
          if (blocksLight(x, bottomY, octant, origin)) {
            if (!blocksLight(x, bottomY + 1, octant, origin)) {
              ++bottomY;
            }
          }
        }
      }
      
      int wasOpaque = -1;
      for (unsigned y = topY; int(y) >= int(bottomY); --y) {
        if (range < 0 || getDistance(x, y) <= range) {
          const bool isOpaque = blocksLight(x, y, octant, origin);
          if (isOpaque) {
            setVisible(x, y, octant, origin);
          } else {
            if (y != topY || top > Slope{y * 4 - 1, x * 4 + 1}) {
              if (y != bottomY || bottom < Slope{y * 4 + 1, x * 4 - 1}) {
                setVisible(x, y, octant, origin);
              }
            }
          }
          
          if (int(x) != range) {
            if (isOpaque) {
              if (wasOpaque == 0) {
                const unsigned nx = x * 2 - blocksLight(x, y + 1, octant, origin);
                const unsigned ny = y * 2 + 1;
                if (top > Slope{ny, nx}) {
                  if (y == bottomY) {
                    bottom = {ny, nx};
                    break;
                  } else {
                    update(octant, origin, range, x + 1, top, {ny, nx});
                  }
                } else {
                  if (y == bottomY) return;
                }
              }
              wasOpaque = 1;
            } else {
              if (wasOpaque == 1) {
                const unsigned nx = x * 2 + blocksLight(x + 1, y + 1, octant, origin);
                const unsigned ny = y * 2 + 1;
                if (bottom >= Slope{ny, nx}) return;
                top = {ny, nx};
              }
              wasOpaque = 0;
            }
          }
        }
      }
      
      if (wasOpaque != 0) break;
    }
  }
};

bool opaque(const Tile tile) {
  switch (tile) {
    case Tile::open_door:
    case Tile::room:
    case Tile::path:
    case Tile::stairs:
      return false;
    case Tile::wall:
    case Tile::closed_door:
      return true;
  }
}

bool opaque(const gfx::Surface<const Tile> tiles, const int x, const int y) {
  if (tiles.contains(x, y)) {
    return opaque(tiles.ref(x, y));
  } else {
    return true;
  }
}

class LightVision final : public Vision<LightVision> {
public:
  LightVision(gfx::Surface<const Tile> tiles, gfx::Surface<bool> lit)
    : tiles{tiles}, lit{lit} {
    assert(tiles.size() == lit.size());
  }

  bool blocksLight(const unsigned x, const unsigned y) const {
    return opaque(tiles, x, y);
  }
  
  void setVisible(const unsigned x, const unsigned y) {
    if (lit.contains(x, y)) {
      lit.ref(x, y) = true;
    }
  }

private:
  gfx::Surface<const Tile> tiles;
  gfx::Surface<bool> lit;
};

class PlayerVision final : public Vision<PlayerVision> {
public:
  PlayerVision(
    const gfx::Surface<const Tile> tiles,
    const gfx::Surface<const bool> lit,
    const gfx::Surface<Visibility> visible
  ) : tiles{tiles}, lit{lit}, visible{visible} {
    assert(tiles.size() == lit.size() && lit.size() == visible.size());
  }
  
  bool blocksLight(const unsigned x, const unsigned y) const {
    return opaque(tiles, x, y);
  }
  
  void setVisible(const unsigned x, const unsigned y) {
    if (lit.contains(x, y)) {
      if (lit.ref(x, y)) {
        visible.ref(x, y) = Visibility::visible;
      }
    }
  }
  
private:
  gfx::Surface<const Tile> tiles;
  gfx::Surface<const bool> lit;
  gfx::Surface<Visibility> visible;
};

class MonsterVision final : public Vision<MonsterVision> {
public:
  MonsterVision(
    const gfx::Surface<const Tile> tiles,
    const gfx::Surface<const bool> lit,
    const gfx::Point playerPos
  ) : tiles{tiles}, lit{lit}, playerPos{playerPos} {}

  bool blocksLight(const unsigned x, const unsigned y) const {
    return opaque(tiles, x, y);
  }
  
  void setVisible(const unsigned x, const unsigned y) {
    if (playerPos.x == int(x) && playerPos.y == int(y)) {
      assert(lit.contains(x, y));
      seen = seen || lit.ref(x, y);
    }
  }

  bool visible() const {
    return seen;
  }

private:
  gfx::Surface<const Tile> tiles;
  gfx::Surface<const bool> lit;
  gfx::Point playerPos;
  bool seen;
};

}

void initializeLight(entt::registry &reg, const gfx::Size size) {
  assert(size.w > 1 && size.h > 1);
  Light &light = reg.set<Light>();
  light.lit = {size};
  gfx::fill(light.lit.view(), false);
  Sight &sight = reg.set<Sight>();
  sight.visibility = {size};
  gfx::fill(sight.visibility.view(), Visibility::unexplored);
}

void updateLight(entt::registry &reg) {
  gfx::Surface<const Tile> tiles = reg.ctx<World>().tiles;
  gfx::Surface<bool> lit = reg.ctx<Light>().lit;
  gfx::fill(lit, false);
  reg.view<Position, LightSource>().each([&](auto pos, auto src) {
    LightVision vision{tiles, lit};
    vision.update(pos.p, src.radius);
  });
}

void updateVisibility(entt::registry &reg) {
  gfx::Surface<const Tile> tiles = reg.ctx<World>().tiles;
  gfx::Surface<const bool> lit = reg.ctx<Light>().lit;
  gfx::Surface<Visibility> visible = reg.ctx<Sight>().visibility;
  gfx::each(visible, [](Visibility &vis) {
    vis = (vis == Visibility::visible ? Visibility::invisible : vis);
  });
  reg.view<Position, Player>().less([&](auto pos) {
    PlayerVision vision{tiles, lit, visible};
    vision.update(pos.p, -1);
  });
}

bool canSee(entt::registry &reg, const gfx::Point from, const gfx::Point to) {
  gfx::Surface<const Tile> tiles = reg.ctx<World>().tiles;
  gfx::Surface<const bool> lit = reg.ctx<Light>().lit;
  MonsterVision vision{tiles, lit, to};
  vision.update(from, -1);
  return vision.visible();
}
