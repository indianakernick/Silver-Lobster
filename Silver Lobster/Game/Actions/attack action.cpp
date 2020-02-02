//
//  attack action.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 1/2/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "attack action.hpp"

#include "rng.hpp"
#include "tags.hpp"
#include <iostream>
#include "combat.hpp"
#include <entt/entity/registry.hpp>

AttackAction::AttackAction(const entt::entity target)
  : target{target} {}

Outcome AttackAction::apply(entt::registry &reg, const entt::entity e) const {
  // TODO: The rng seed should be in a context variable
  // use std::random_device{}() at the start of each level
  static RNG rng{1234};
  
  if (!reg.has<Attack>(e)) return false;
  const Attack attack = reg.get<Attack>(e);
  // TODO: partial hits
  // there could be a chance of doing full damage then a smaller chance of doing
  // half damage
  if (!rng.percent(attack.accuracy)) {
    if (reg.has<Player>(target)) {
      std::cout << "Ghost missed\n";
    } else if (reg.has<Player>(e)) {
      std::cout << "Player missed\n";
    }
    return true;
  }
  
  if (auto *dodge = reg.try_get<Dodge>(target); dodge) {
    if (rng.percent(dodge->chance)) {
      if (reg.has<Player>(target)) {
        std::cout << "Player dodged\n";
      } else if (reg.has<Player>(e)) {
        std::cout << "Ghost missed\n";
      }
      return true;
    }
  }
  
  int damage = attack.damage;
  if (auto *armour = reg.try_get<Armour>(target); armour) {
    if (armour->durability > 0) {
      damage -= (damage * armour->percent) / 100;
      --armour->durability;
      if (armour->durability == 0) {
        if (reg.has<Player>(target)) {
          std::cout << "Player armour broke\n";
        } else if (reg.has<Player>(e)) {
          std::cout << "Ghost armour broke\n";
        }
      }
    }
  }
  
  if (reg.has<Player>(target)) {
    std::cout << "Ghost hit player\n";
  } else if (reg.has<Player>(e)) {
    std::cout << "Player hit ghost\n";
  }
  
  int &health = reg.get<Health>(target).amount;
  health -= damage;
  if (reg.has<Player>(target)) {
    std::cout << "Player health: " << health << '\n';
  } else if (reg.has<Player>(e)) {
    std::cout << "Ghost health: " << health << '\n';
  }
  
  if (health <= 0) {
    if (reg.has<Player>(target)) {
      std::cout << "Player died\n";
    } else if (reg.has<Player>(e)) {
      std::cout << "Ghost died\n";
    }
    // TODO: order of turns get messed up when this happens
    reg.destroy(target);
  }
  
  return true;
}
