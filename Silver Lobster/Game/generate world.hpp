//
//  generate world.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef generate_world_hpp
#define generate_world_hpp

#include <entt/entity/fwd.hpp>
#include <Graphics/geometry.hpp>

void initializeWorld(entt::registry &, gfx::Size);
void generateTerrain(entt::registry &);

#endif
