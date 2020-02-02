//
//  sprite.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef sprite_hpp
#define sprite_hpp

#include "reflection.hpp"

struct Sprite {
  int x, y;
};

REFL(Sprite,
  REFL_DATA(x)
  REFL_DATA(y)
);

#endif
