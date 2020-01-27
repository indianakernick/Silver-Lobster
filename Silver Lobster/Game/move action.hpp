//
//  move action.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef move_action_hpp
#define move_action_hpp

#include "dir.hpp"
#include "action.hpp"

class MoveAction final : public Action {
public:
  explicit MoveAction(Dir);

  Outcome apply(entt::registry &, entt::entity) override;

private:
  Dir dir;
};

#endif
