//
//  world generation.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef world_generation_hpp
#define world_generation_hpp

#include <entt/entity/fwd.hpp>
#include <Graphics/geometry.hpp>

void initializeWorld(entt::registry &, gfx::Size);
void generateTerrain(entt::registry &, uint64_t);

#endif
