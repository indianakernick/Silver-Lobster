//
//  update light.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef update_light_hpp
#define update_light_hpp

#include <entt/entity/fwd.hpp>

void initializeLight(entt::registry &, int, int);
void updateLight(entt::registry &);
void illuminate(entt::registry &);

#endif
