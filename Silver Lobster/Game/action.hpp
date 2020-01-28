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
  Outcome(const bool success)
    : alternative{}, succeeded{success} {}
  
  template <typename A>
  Outcome(std::unique_ptr<A> alt)
    : alternative{std::move(alt)}, succeeded{true} {}
  
  std::unique_ptr<Action> alternative;
  bool succeeded;
};

class Action {
public:
  virtual ~Action();
  
  virtual Outcome apply(entt::registry &, entt::entity) const = 0;
};

#endif
