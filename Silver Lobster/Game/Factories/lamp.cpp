//
//  lamp.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "lamp.hpp"

#include "sprite.hpp"
#include "position.hpp"
#include "light source.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeLamp(entt::registry &reg, const gfx::Point pos) {
  entt::entity e = reg.create();
  reg.assign<Position>(e, pos);
  reg.assign<Sprite>(e, 85, 255);
  reg.assign<LightSource>(e, 12);
  return e;
}
