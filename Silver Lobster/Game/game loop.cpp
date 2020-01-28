//
//  game loop.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "game loop.hpp"

#include "turn.hpp"
#include "speed.hpp"
#include "brain.hpp"
#include "render world.hpp"
#include "update light.hpp"
#include "render entities.hpp"
#include <entt/entity/registry.hpp>

void stepGame(entt::registry &reg) {
  Turn &turn = reg.ctx_or_set<Turn>(size_t{});
  auto speedView = reg.view<Speed>();
  if (turn.i >= speedView.size()) {
    turn.i = 0;
  }
  
  const entt::entity e = speedView[turn.i];
  Speed &speed = speedView.get(e);
  if (speed.sum >= Speed::max) {
    if (!reg.has<Brain>(e)) return;
    std::unique_ptr<Action> action = reg.get<Brain>(e).b->decide(reg, e);
    if (action == nullptr) return;
    while (true) {
      Outcome outcome = action->apply(reg, e);
      if (!outcome.succeeded) return;
      if (!outcome.alternative) break;
      action = std::move(outcome.alternative);
    }
    speed.sum = speed.sum - Speed::max + speed.delta;
  } else {
    speed.sum += speed.delta;
  }
  ++turn.i;
  
  updateLight(reg);
}

void renderGame(const entt::registry &reg) {
  renderWorld(reg);
  renderEntities(reg);
}
