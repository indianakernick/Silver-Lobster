//
//  game loop.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef game_loop_hpp
#define game_loop_hpp

#include <entt/entity/fwd.hpp>

bool stepGame(entt::registry &);
void renderGame(const entt::registry &);

#endif
