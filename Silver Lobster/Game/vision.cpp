//
//  vision.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "vision.hpp"

#include <cmath>
#include "light.hpp"
#include "world.hpp"
#include <Graphics/each.hpp>

void initializeLight(Light &light, const int width, const int height) {
  assert(width > 1);
  assert(height > 1);
  light.visibilityStorage = std::make_unique<Visibility[]>(width * height);
  light.visibility = {light.visibilityStorage.get(), width, width, height};
}

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

class Vision {
public:
  Vision(Light &light, const World &world)
    : visible{light.visibility}, tiles{world.tiles} {
    assert(visible.size() == tiles.size());
  }
  
  void update(const gfx::Point origin, const VisParams &params) {
    clear();
    setVisible(origin.x, origin.y);
    for (unsigned octant = 0; octant != 8; ++octant) {
      update(octant, origin, params.range, 1, {1, 1}, {0, 1});
    }
  }
  
private:
  gfx::Surface<Visibility> visible;
  gfx::Surface<const Tile> tiles;
  
  void clear() {
    gfx::each(visible, [](Visibility &vis) {
      vis = (vis == Visibility::visible ? Visibility::invisible : vis);
    });
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
  
  bool blocksLight(const unsigned x, const unsigned y, const unsigned octant, gfx::Point origin) {
    origin += octantOffset(x, y, octant);
    return blocksLight(origin.x, origin.y);
  }
  
  void setVisible(const unsigned x, const unsigned y, const unsigned octant, gfx::Point origin) {
    origin += octantOffset(x, y, octant);
    setVisible(origin.x, origin.y);
  }
  
  bool blocksLight(const unsigned x, const unsigned y) {
    if (!tiles.contains(x, y)) {
      return true;
    }
    switch (tiles.ref(x, y)) {
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
  
  void setVisible(const unsigned x, const unsigned y) {
    if (visible.contains(x, y)) {
      visible.ref(x, y) = Visibility::visible;
    }
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

}

void updateLight(
  Light &light,
  const World &world,
  const gfx::Point origin,
  const VisParams &params
) {
  Vision vision{light, world};
  vision.update(origin, params);
}
