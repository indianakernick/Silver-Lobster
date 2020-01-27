//
//  action.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef action_hpp
#define action_hpp

#include <memory>
#include <entt/entity/fwd.hpp>

class Action;

struct Outcome {
  Outcome(bool);
  Outcome(std::unique_ptr<Action>);
  
  std::unique_ptr<Action> alternative;
  bool succeeded;
};

class Action {
public:
  virtual ~Action() = default;
  
  virtual Outcome apply(entt::registry &, entt::entity) = 0;
};

#endif
