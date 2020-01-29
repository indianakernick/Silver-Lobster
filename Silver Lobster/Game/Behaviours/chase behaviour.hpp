//
//  chase behaviour.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef chase_behaviour_hpp
#define chase_behaviour_hpp

#include "behaviour.hpp"

class ChaseBehaviour final : public Behaviour {
public:
  std::unique_ptr<Action> decide(entt::registry &, entt::entity) override;
};

#endif
