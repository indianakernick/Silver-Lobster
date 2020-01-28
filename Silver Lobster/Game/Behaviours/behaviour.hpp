//
//  behaviour.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 28/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef behaviour_hpp
#define behaviour_hpp

#include <memory>
#include "action.hpp"
#include <entt/entity/fwd.hpp>

class Behaviour {
public:
  virtual ~Behaviour();
  
  virtual std::unique_ptr<Action> decide(entt::registry &, entt::entity) = 0;
};

#endif
