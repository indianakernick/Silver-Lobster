//
//  combat.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 1/2/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef combat_hpp
#define combat_hpp

struct Health {
  int amount;
};

struct Dodge {
  int chance;
};

struct Armour {
  int percent;
  int durability;
};

struct Attack {
  int damage;
  int accuracy;
};

#endif
