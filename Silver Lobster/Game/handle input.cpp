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

void setMove(entt::registry &reg, const Dir dir) {
  reg.view<Player>().less([&](entt::entity player) {
    reg.assign<Move>(player, dir);
  });
}

}

bool handleKeyDown(entt::registry &reg, const SDL_Scancode key) {
  switch (key) {
    case SDL_SCANCODE_I:
    case SDL_SCANCODE_8:
    case SDL_SCANCODE_UP:
      setMove(reg, Dir::up);
      return true;
      
    case SDL_SCANCODE_O:
    case SDL_SCANCODE_9:
      setMove(reg, Dir::up_right);
      return true;
      
    case SDL_SCANCODE_L:
    case SDL_SCANCODE_6:
    case SDL_SCANCODE_RIGHT:
      setMove(reg, Dir::right);
      return true;
      
    case SDL_SCANCODE_PERIOD:
    case SDL_SCANCODE_3:
      setMove(reg, Dir::down_right);
      return true;
      
    case SDL_SCANCODE_COMMA:
    case SDL_SCANCODE_2:
    case SDL_SCANCODE_DOWN:
      setMove(reg, Dir::down);
      return true;
    
    case SDL_SCANCODE_M:
    case SDL_SCANCODE_1:
      setMove(reg, Dir::down_left);
      return true;
    
    case SDL_SCANCODE_J:
    case SDL_SCANCODE_4:
    case SDL_SCANCODE_LEFT:
      setMove(reg, Dir::left);
      return true;
    
    case SDL_SCANCODE_U:
    case SDL_SCANCODE_7:
      setMove(reg, Dir::up_left);
      return true;
    
    case SDL_SCANCODE_K:
    case SDL_SCANCODE_5:
      setMove(reg, Dir::none);
      return true;
      
    default:
      return false;
  }
}
