//
//  attack action.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 1/2/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef attack_action_hpp
#define attack_action_hpp

#include "action.hpp"

class AttackAction final : public Action {
public:
  explicit AttackAction(entt::entity);

  Outcome apply(entt::registry &, entt::entity) const override;

private:
  entt::entity target;
};

#endif
