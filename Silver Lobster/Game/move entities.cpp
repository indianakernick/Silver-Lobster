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
      case Dir::up:
        --pos.y;
        break;
      case Dir::up_right:
        ++pos.x;
        --pos.y;
        break;
      case Dir::right:
        ++pos.x;
        break;
      case Dir::down_right:
        ++pos.x;
        ++pos.y;
        break;
      case Dir::down:
        ++pos.y;
        break;
      case Dir::down_left:
        --pos.x;
        ++pos.y;
        break;
      case Dir::left:
        --pos.x;
        break;
      case Dir::up_left:
        --pos.x;
        --pos.y;
        break;
      case Dir::none:
        break; // rest
    }
  });
  reg.view<Player, Move>().less([&](entt::entity player, auto move) {
    if (move.dir != Dir::none) {
      reg.assign<UpdateLight>(player);
    }
  });
  reg.reset<Move>();
}
