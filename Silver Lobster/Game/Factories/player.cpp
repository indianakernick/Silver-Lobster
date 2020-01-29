//
//  player.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 28/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "player.hpp"

#include "tags.hpp"
#include "brain.hpp"
#include "speed.hpp"
#include "sprite.hpp"
#include "position.hpp"
#include "light source.hpp"
#include "player behaviour.hpp"
#include <entt/entity/registry.hpp>

entt::entity makePlayer(entt::registry &reg, const gfx::Point pos) {
  entt::entity e = reg.create();
  reg.assign<Player>(e);
  reg.assign<Position>(e, pos);
  reg.assign<Sprite>(e, 459, 0);
  reg.assign<Speed>(e, Speed::max);
  reg.assign<Brain>(e, std::make_unique<PlayerBehaviour>());
  reg.assign<LightSource>(e, 1);
  return e;
}
