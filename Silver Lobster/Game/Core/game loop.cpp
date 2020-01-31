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
#include "scope time.hpp"
#include "field of view.hpp"
#include "world rendering.hpp"
#include "sprite rendering.hpp"
#include <entt/entity/registry.hpp>

bool stepGame(entt::registry &reg) {
  SCOPE_TIME("stepGame");

  Turn &turn = reg.ctx_or_set<Turn>(size_t{});
  auto speedView = reg.view<Speed>();
  if (turn.i >= speedView.size()) {
    turn.i = 0;
  }
  
  const entt::entity e = speedView[turn.i];
  Speed &speed = speedView.get(e);
  if (speed.sum >= Speed::max) {
    if (!reg.has<Brain>(e)) return false;
    std::unique_ptr<Action> action = reg.get<Brain>(e).b->decide(reg, e);
    if (action == nullptr) return false;
    while (true) {
      Outcome outcome = action->apply(reg, e);
      if (!outcome.succeeded) return false;
      if (!outcome.alternative) break;
      action = std::move(outcome.alternative);
    }
    speed.sum = speed.sum - Speed::max + speed.delta;
  } else {
    speed.sum += speed.delta;
  }
  ++turn.i;
  
  updateLight(reg);
  updateVisibility(reg);
  return true;
}

void renderGame(const entt::registry &reg) {
  SCOPE_TIME("renderGame");
  
  renderWorld(reg);
  renderSprites(reg);
}
