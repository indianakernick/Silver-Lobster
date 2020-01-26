//
//  vision.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef vision_hpp
#define vision_hpp

#include <Graphics/geometry.hpp>

struct Light;
struct World;

struct VisParams {
  int range;
};

void initializeLight(Light &, int, int);
void updateLight(Light &, const World &, gfx::Point, const VisParams &);
void illuminate(Light &);

#endif
