//
//  rng.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 30/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef rng_hpp
#define rng_hpp

#include <cstdint>
#include <Graphics/geometry.hpp>

class RNG {
public:
  using result_type = uint64_t;
  
  static constexpr result_type min() noexcept {
    return 0;
  }
  static constexpr result_type max() noexcept {
    return ~result_type{};
  }

  // RNG();
  explicit RNG(result_type);

  void seed(result_type);
  result_type operator()();
  
  int range(int, int);
  bool percent(int);
  
  gfx::Point point(gfx::Rect);
  gfx::Point point(gfx::Size);
  gfx::Size size(int, int);
  
  gfx::Point oddPoint(gfx::Size);
  gfx::Size oddSize(int, int);

private:
  uint64_t seed_;
};

#endif
