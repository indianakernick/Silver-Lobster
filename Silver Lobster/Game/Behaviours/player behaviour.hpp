//
//  player behaviour.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 28/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef player_behaviour_hpp
#define player_behaviour_hpp

#include "behaviour.hpp"

class PlayerBehaviour final : public Behaviour {
public:
  std::unique_ptr<Action> decide(entt::registry &, entt::entity) override;
  void inject(std::unique_ptr<Action>);

private:
  std::unique_ptr<Action> next;
};

#endif
