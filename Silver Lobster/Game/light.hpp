//
//  light.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include <memory>
#include <Graphics/surface.hpp>

enum class Visibility {
  unexplored,
  invisible,
  visible
};

struct Light {
  std::unique_ptr<Visibility[]> visibilityStorage;
  gfx::Surface<Visibility> visibility;
};

#endif
