//
//  ghost.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef ghost_hpp
#define ghost_hpp

#include <entt/entity/fwd.hpp>
#include <Graphics/geometry.hpp>

entt::entity makeGhost(entt::registry &, gfx::Point);

#endif
