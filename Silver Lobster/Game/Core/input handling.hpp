//
//  input handling.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef input_handling_hpp
#define input_handling_hpp

#include <SDL2/SDL_scancode.h>
#include <entt/entity/fwd.hpp>

bool handleKeyDown(entt::registry &, SDL_Scancode);

#endif
