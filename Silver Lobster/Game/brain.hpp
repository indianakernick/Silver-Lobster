//
//  brain.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 28/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef brain_hpp
#define brain_hpp

#include "behaviour.hpp"

struct Brain {
  std::unique_ptr<Behaviour> b;
};

#endif
