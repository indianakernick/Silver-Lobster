//
//  text rendering.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 3/2/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "text rendering.hpp"

#include "sdl check.hpp"
#include "sdl delete.hpp"
#include "scope time.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2_ttf/SDL_ttf.h>

void renderText(
  SDL_Renderer *renderer,
  _TTF_Font *font,
  const char *text,
  const int x,
  const int y
) {
  SCOPE_TIME("renderText");

  // TODO: This is so incredibly inefficient
  const SDL_Color white = {255, 255, 255, 255};
  SDL::Surface surf{SDL_CHECK(TTF_RenderText_Solid(font, text, white))};
  SDL::Texture tex{SDL_CHECK(SDL_CreateTextureFromSurface(renderer, surf.get()))};
  int width, height;
  SDL_CHECK(SDL_QueryTexture(tex.get(), nullptr, nullptr, &width, &height));
  const SDL_Rect dst = {x, y, width, height};
  SDL_CHECK(SDL_RenderCopy(renderer, tex.get(), nullptr, &dst));
}
