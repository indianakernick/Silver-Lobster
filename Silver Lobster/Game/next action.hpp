//
//  next action.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef next_action_hpp
#define next_action_hpp

#include "action.hpp"

struct NextAction {
  std::unique_ptr<Action> next;
};

#endif
