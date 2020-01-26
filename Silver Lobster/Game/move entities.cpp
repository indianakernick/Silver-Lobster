//
//  move entities.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "move entities.hpp"

#include "tags.hpp"
#include "intents.hpp"
#include "position.hpp"
#include <entt/entity/registry.hpp>

void moveEntities(entt::registry &reg) {
  reg.view<Position, Move>().each([](auto &pos, auto move) {
    switch (move.dir) {
      case Grid::Dir::up:
        --pos.y;
        break;
      case Grid::Dir::right:
        ++pos.x;
        break;
      case Grid::Dir::down:
        ++pos.y;
        break;
      case Grid::Dir::left:
        --pos.x;
        break;
      case Grid::Dir::none:
        break; // rest
    }
  });
  reg.view<Player, Move>().less([&](entt::entity player, auto move) {
    if (move.dir != Grid::Dir::none) {
      reg.assign<UpdateLight>(player);
    }
  });
  reg.reset<Move>();
}
