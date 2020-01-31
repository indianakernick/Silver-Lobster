//
//  scope time.hpp
//  Animera
//
//  Created by Indi Kernick on 1/12/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#ifndef scope_time_hpp
#define scope_time_hpp

#ifdef ENABLE_SCOPE_TIME

#include <chrono>

class ScopeTime final {
public:
  explicit ScopeTime(const char *name) {
    push(name);
    start = Clock::now();
  }
  ~ScopeTime() {
    pop(start, Clock::now());
  }
  
  static void print();
  static void reset();
  
private:
  using Clock = std::chrono::high_resolution_clock;

  Clock::time_point start;

  struct TreeNode;
  
  static TreeNode tree;
  static TreeNode *current;
  
  static void push(const char *);
  static void pop(Clock::time_point, Clock::time_point) noexcept;
  static void printImpl(const TreeNode &, int);
};

#define SCOPE_TIME_IMPL2(NAME, LINE) const ScopeTime scope_time_##LINE {NAME}
#define SCOPE_TIME_IMPL(NAME, LINE) SCOPE_TIME_IMPL2(NAME, LINE)
#define SCOPE_TIME(NAME) SCOPE_TIME_IMPL(NAME, __LINE__)
#define NO_SCOPE_TIME(NAME) SCOPE_TIME(nullptr)
#define SCOPE_TIME_PRINT() ScopeTime::print()
#define SCOPE_TIME_RESET() ScopeTime::reset()

#else

#define SCOPE_TIME(NAME)
#define NO_SCOPE_TIME(NAME)
#define SCOPE_TIME_PRINT()
#define SCOPE_TIME_RESET()

#endif

#endif
