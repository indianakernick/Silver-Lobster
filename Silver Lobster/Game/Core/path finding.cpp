//
//  path finding.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "path finding.hpp"

#include <queue>
#include "world.hpp"
#include "dir point.hpp"
#include <entt/entity/registry.hpp>

namespace {

struct Node {
  gfx::Point pos;
  gfx::Point prev;
  int steps;
  float cost;
};

struct CompareNodes {
  bool operator()(const Node &lhs, const Node &rhs) const noexcept {
    return lhs.cost > rhs.cost;
  }
};

struct NodeQueue : std::priority_queue<Node, std::vector<Node>, CompareNodes> {
  using priority_queue::c;
};

/*
struct Policy {
  // what is the distance between these points?
  float distance(gfx::Point, gfx::Point) const;
  // is it possible to walk to this point by walking in this direction?
  bool walkable(gfx::Point, Dir) const;
  
  // this node is now being processed
  // (makes it possible to get list of points that make up the path)
  void next(const Node &);
  
  // no path was found
  void fail();
  // a path was found
  void succeed(Node &);
};
*/

template <typename Policy>
void astar(Policy &policy, const gfx::Point from, const gfx::Point to) {
  // TODO: this never returns when there is no route
  
  NodeQueue queue;
  queue.push({to, to, 0, policy.distance(from, to)});
  
  while (true) {
    if (queue.empty()) {
      policy.fail();
      return;
    } else if (queue.top().pos == from) {
      policy.succeed(queue.top());
      return;
    }
    
    const Node top = queue.top();
    queue.pop();
    policy.next(top);
    const int neighborSteps = top.steps + 1;
    
    for (const Dir dir : all_dirs) {
      const gfx::Point neighborPos = top.pos + toPoint(dir);
      if (neighborPos == top.prev) continue;
      if (!policy.walkable(neighborPos, dir)) continue;
      
      const Node neighbor = {
        neighborPos,
        top.pos,
        neighborSteps,
        neighborSteps + policy.distance(neighborPos, from)
      };
      bool found = false;
      
      for (auto n = queue.c.begin(); n != queue.c.end(); ++n) {
        if (n->pos == neighborPos) {
          found = true;
          if (neighbor.steps < n->steps) {
            // TODO: Could remove n
            // but that's a bit tricky
            queue.push(neighbor);
          }
          break;
        }
      }
      
      if (!found) {
        queue.push(neighbor);
      }
    }
  }
}

class MonsterPolicy {
public:
  static float distance(const gfx::Point a, const gfx::Point b) {
    const gfx::Point diff = a - b;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
  }
  
  bool walkable(const gfx::Point dest, Dir) const {
    if (!tiles.contains(dest)) return false;
    switch (tiles.ref(dest)) {
      case Tile::open_door:
      case Tile::room:
      case Tile::path:
      case Tile::stairs:
        return true;
      case Tile::wall:
      case Tile::closed_door:
        return false;
    }
  }
  
  void next(const Node &) {}
  
  void fail() {
    dir = Dir::none;
  }
  
  void succeed(const Node &node) {
    dir = fromPoint(node.prev - node.pos);
  }
  
  explicit MonsterPolicy(const gfx::Surface<const Tile> tiles)
    : tiles{tiles} {}
  
  Dir moveDir() const {
    return dir;
  }

private:
  gfx::Surface<const Tile> tiles;
  Dir dir = Dir::none;
};

}

Dir findPath(entt::registry &reg, const gfx::Point from, const gfx::Point to) {
  MonsterPolicy policy{reg.ctx<World>().tiles};
  astar(policy, from, to);
  return policy.moveDir();
}
