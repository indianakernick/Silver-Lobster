//
//  light.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include "image.hpp"

enum class Visibility {
  unexplored,
  invisible,
  visible
};

struct Light {
  Image<Visibility> visibility;
};

#endif
