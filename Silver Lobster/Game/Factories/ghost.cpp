//
//  ghost.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "ghost.hpp"

#include "brain.hpp"
#include "speed.hpp"
#include "sprite.hpp"
#include "combat.hpp"
#include "position.hpp"
#include "chase behaviour.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeGhost(entt::registry &reg, const gfx::Point pos) {
  entt::entity e = reg.create();
  reg.assign<Position>(e, pos);
  reg.assign<Sprite>(e, 442, 102);
  reg.assign<Speed>(e, Speed::max / 2);
  reg.assign<Brain>(e, std::make_unique<ChaseBehaviour>());
  reg.assign<Health>(e, 20);
  reg.assign<Dodge>(e, 30);
  reg.assign<Attack>(e, 2, 90);
  return e;
}
