//
//  dir set.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef dir_set_hpp
#define dir_set_hpp

#include "dir.hpp"

class DirSet {
public:
  constexpr DirSet operator&(const DirSet other) const noexcept {
    return bits & other.bits;
  }
  constexpr DirSet operator|(const DirSet other) const noexcept {
    return bits | other.bits;
  }
  constexpr DirSet operator^(const DirSet other) const noexcept {
    return bits ^ other.bits;
  }
  constexpr DirSet operator~() const noexcept {
    return bits ^ 0xFF;
  }
  
  constexpr DirSet &operator&=(const DirSet other) noexcept {
    bits &= other.bits;
    return *this;
  }
  constexpr DirSet &operator|=(const DirSet other) noexcept {
    bits |= other.bits;
    return *this;
  }
  constexpr DirSet &operator^=(const DirSet other) noexcept {
    bits ^= other.bits;
    return *this;
  }
  
  constexpr bool operator==(const DirSet other) const noexcept {
    return bits == other.bits;
  }
  constexpr bool operator!=(const DirSet other) const noexcept {
    return bits != other.bits;
  }
  
  constexpr void set(const Dir dir) noexcept {
    bits |= bit(dir);
  }
  constexpr void reset(const Dir dir) noexcept {
    bits &= ~bit(dir);
  }
  constexpr void flip(const Dir dir) noexcept {
    bits ^= bit(dir);
  }
  constexpr bool test(const Dir dir) const noexcept {
    return bits & bit(dir);
  }
  
  constexpr void set() noexcept {
    bits = 0xFF;
  }
  constexpr void reset() noexcept {
    bits = 0;
  }
  constexpr void flip() noexcept {
    bits ^= 0xFF;
  }
  
  constexpr void setCardinal() noexcept {
    bits |= 0x55;
  }
  constexpr void resetCardinal() noexcept {
    bits &= 0xAA;
  }
  constexpr void flipCardinal() noexcept {
    bits ^= 0x55;
  }
  
  constexpr bool all() const noexcept {
    return bits == 0xFF;
  }
  constexpr bool any() const noexcept {
    return bits;
  }
  constexpr bool none() const noexcept {
    return bits == 0;
  }
  constexpr uint8_t count() const noexcept {
    return popcount(bits);
  }
  
  constexpr DirSet cardinal() const noexcept {
    return bits & 0x55;
  }
  
  constexpr bool allCardinal() const noexcept {
    return (bits & 0x55) == 0x55;
  }
  constexpr bool anyCardinal() const noexcept {
    return bits & 0x55;
  }
  constexpr bool noneCardinal() const noexcept {
    return (bits & 0x55) == 0;
  }
  constexpr uint8_t countCardinal() const noexcept {
    return popcount(bits & 0x55);
  }
  
  constexpr DirSet() noexcept = default;
  
  template <size_t Size>
  constexpr explicit DirSet(const Dir (&dirs)[Size]) noexcept {
    for (const Dir dir : dirs) {
      set(dir);
    }
  }
  
private:
  uint8_t bits = 0;
  
  constexpr DirSet(const uint8_t bits)
    : bits{bits} {}
    
  static constexpr int bit(const Dir dir) noexcept {
    return 1 << static_cast<uint8_t>(dir);
  }
  
  static constexpr uint8_t popcount(uint8_t bits) noexcept {
    // https://stackoverflow.com/a/30692782/4093378
    bits = bits - ((bits >> 1) & 0x55);
    bits = (bits & 0x33) + ((bits >> 2) & 0x33);
    return (((bits + (bits >> 4)) & 0x0F) * 0x01);
  }
};

#endif
