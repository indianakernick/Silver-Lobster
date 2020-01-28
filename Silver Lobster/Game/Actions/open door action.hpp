//
//  open door action.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 28/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef open_door_action_hpp
#define open_door_action_hpp

#include "dir.hpp"
#include "action.hpp"

class OpenDoorAction final : public Action {
public:
  OpenDoorAction();
  OpenDoorAction(Dir);

  Outcome apply(entt::registry &, entt::entity) const override;

private:
  Dir dir;
};

#endif
