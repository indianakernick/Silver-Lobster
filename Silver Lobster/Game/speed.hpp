//
//  speed.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 27/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef speed_hpp
#define speed_hpp

#include "reflection.hpp"

struct Speed {
  int delta;
  int sum = 0;
  
  static constexpr int max = 360;
};

REFL(Speed,
  REFL_DATA(delta)
  REFL_DATA(sum)
);

#endif
