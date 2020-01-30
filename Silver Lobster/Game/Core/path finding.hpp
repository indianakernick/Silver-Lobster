//
//  path finding.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef path_finding_hpp
#define path_finding_hpp

#include "dir.hpp"
#include <entt/entity/fwd.hpp>
#include <Graphics/geometry.hpp>

Dir findPath(entt::registry &, gfx::Point, gfx::Point);

#endif
