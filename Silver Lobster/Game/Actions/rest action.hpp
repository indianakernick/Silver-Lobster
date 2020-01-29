//
//  rest action.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef rest_action_hpp
#define rest_action_hpp

#include "action.hpp"

class RestAction final : public Action {
public:
  Outcome apply(entt::registry &, entt::entity) const override;
};

#endif
