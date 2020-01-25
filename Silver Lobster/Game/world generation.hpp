//
//  world generation.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef world_generation_hpp
#define world_generation_hpp

#include <cstdint>

struct World;

struct GenParams {
  uint64_t seed;
  int roomSizeMin;
  int roomSizeMax;
  int roomDensity;
  int mazeStraightness;
  int connectionRedundancy;
};

void initializeWorld(World &, int, int);
void generateTerrain(World &, const GenParams &);

#endif
