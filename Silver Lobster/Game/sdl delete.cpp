//
//  sdl delete.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "sdl delete.hpp"

#include <SDL2/SDL_render.h>

void SDL::DeleteWindow::operator()(SDL_Window *ptr) const noexcept {
  SDL_DestroyWindow(ptr);
}

void SDL::DeleteTexture::operator()(SDL_Texture *ptr) const noexcept {
  SDL_DestroyTexture(ptr);
}

void SDL::DeleteRenderer::operator()(SDL_Renderer *ptr) const noexcept {
  SDL_DestroyRenderer(ptr);
}
