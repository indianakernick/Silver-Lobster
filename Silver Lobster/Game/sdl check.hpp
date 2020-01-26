//
//  sdl check.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 24/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef sdl_check_hpp
#define sdl_check_hpp

#include <stdexcept>
#include <SDL2/SDL_error.h>

namespace SDL {

class Error final : std::runtime_error {
public:
  Error(const char *error) noexcept
    : std::runtime_error{error} {}
};

[[noreturn]] inline void raise() {
  const char *error = SDL_GetError();
  throw Error{error};
}

template <typename Ptr>
[[nodiscard]] Ptr *check(Ptr *ptr) {
  if (ptr == nullptr) {
    raise();
  } else {
    return ptr;
  }
}

inline void check(const int code) {
  if (code == -1) raise();
}

}

#ifdef DISABLE_SDL_ERROR_CHECK
#  define SDL_CHECK(EXPR) EXPR
#else
#  define SDL_CHECK(EXPR) ::SDL::check(EXPR)
#endif

#endif
