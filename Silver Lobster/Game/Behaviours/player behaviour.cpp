//
//  player behaviour.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 28/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "player behaviour.hpp"

std::unique_ptr<Action> PlayerBehaviour::decide(entt::registry &, entt::entity) {
  return std::exchange(next, nullptr);
}

void PlayerBehaviour::inject(std::unique_ptr<Action> action) {
  next = std::move(action);
}
