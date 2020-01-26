//
//  texture loading.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "texture loading.hpp"

#include <memory>
#include "sdl.hpp"
#include "stb_image.h"
#include <SDL2/SDL_render.h>

namespace {

struct STBI_Deleter {
  void operator()(stbi_uc *ptr) const noexcept {
    stbi_image_free(ptr);
  }
};

}

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path) {
  int width, height;
  std::unique_ptr<stbi_uc, STBI_Deleter> data{stbi_load(
    path, &width, &height, nullptr, 4
  )};
  if (data == nullptr) {
    throw std::runtime_error{stbi_failure_reason()};
  }
  SDL_Texture *tex = SDL_CHECK(SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ABGR8888,
    SDL_TEXTUREACCESS_STATIC,
    width, height
  ));
  SDL_CHECK(SDL_UpdateTexture(tex, nullptr, data.get(), width * 4));
  return tex;
}
