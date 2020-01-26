//
//  handle input.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "handle input.hpp"

#include "tags.hpp"
#include "intents.hpp"
#include <entt/entity/registry.hpp>

namespace {

void setMove(entt::registry &reg, const Grid::Dir dir) {
  reg.view<Player>().less([&](entt::entity player) {
    reg.assign<Move>(player, dir);
  });
}

}

bool handleKeyDown(entt::registry &reg, const SDL_Scancode key) {
  switch (key) {
    case SDL_SCANCODE_UP:
      setMove(reg, Grid::Dir::up);
      return true;
    case SDL_SCANCODE_RIGHT:
      setMove(reg, Grid::Dir::right);
      return true;
    case SDL_SCANCODE_DOWN:
      setMove(reg, Grid::Dir::down);
      return true;
    case SDL_SCANCODE_LEFT:
      setMove(reg, Grid::Dir::left);
      return true;
    default:
      return false;
  }
}
