//
//  chase behaviour.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "chase behaviour.hpp"

#include "tags.hpp"
#include "position.hpp"
#include "rest action.hpp"
#include "move action.hpp"
#include "path finding.hpp"
#include "field of view.hpp"
#include <entt/entity/registry.hpp>

std::unique_ptr<Action> ChaseBehaviour::decide(entt::registry &reg, entt::entity e) {
  const gfx::Point monster = reg.get<Position>(e).p;
  gfx::Point player;
  reg.view<Position, Player>().less([&](auto pos) {
    player = pos.p;
  });
  if (canSee(reg, monster, player)) {
    return std::make_unique<MoveAction>(findPath(reg, monster, player));
  } else {
    return std::make_unique<RestAction>();
  }
}
