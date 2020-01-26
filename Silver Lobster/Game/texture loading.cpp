//
//  texture loading.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "texture loading.hpp"

#include <memory>
#include "stb_image.h"
#include "sdl check.hpp"
#include <SDL2/SDL_render.h>

namespace {

struct DeleteImage {
  void operator()(stbi_uc *ptr) const noexcept {
    stbi_image_free(ptr);
  }
};

using Image = std::unique_ptr<stbi_uc, DeleteImage>;

class ImageError final : std::runtime_error {
public:
  ImageError(const char *error) noexcept
    : std::runtime_error{error} {}
};

}

SDL::Texture loadTexture(SDL_Renderer *renderer, const char *path) {
  int width, height;
  Image data{stbi_load(path, &width, &height, nullptr, 4)};
  if (data == nullptr) {
    throw ImageError{stbi_failure_reason()};
  }
  SDL::Texture tex{SDL_CHECK(SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ABGR8888,
    SDL_TEXTUREACCESS_STATIC,
    width, height
  ))};
  SDL_CHECK(SDL_UpdateTexture(tex.get(), nullptr, data.get(), width * 4));
  return tex;
}
