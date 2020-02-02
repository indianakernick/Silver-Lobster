//
//  position.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef position_hpp
#define position_hpp

#include "reflection.hpp"
#include <Graphics/geometry.hpp>

struct Position {
  gfx::Point p;
};

REFL(Position,
  REFL_DATA(p)
);

#endif
