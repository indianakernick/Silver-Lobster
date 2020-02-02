//
//  combat.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 1/2/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef combat_hpp
#define combat_hpp

#include "reflection.hpp"

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

REFL(Health,
  REFL_DATA(amount)
);

REFL(Dodge,
  REFL_DATA(chance)
);

REFL(Armour,
  REFL_DATA(percent)
  REFL_DATA(durability)
);

REFL(Attack,
  REFL_DATA(damage)
  REFL_DATA(accuracy)
);

#endif
