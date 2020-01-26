//
//  world rendering.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "world rendering.hpp"

#include "sdl.hpp"
#include "light.hpp"
#include "world.hpp"
#include <SDL2/SDL_render.h>

namespace {

void setColorMod(SDL_Texture *tex, const Visibility vis) {
  switch (vis) {
    case Visibility::unexplored:
      SDL_CHECK(SDL_SetTextureColorMod(tex, 0, 0, 0));
      break;
    case Visibility::invisible:
      SDL_CHECK(SDL_SetTextureColorMod(tex, 127, 127, 127));
      break;
    case Visibility::visible:
      SDL_CHECK(SDL_SetTextureColorMod(tex, 255, 255, 255));
      break;
  }
}

SDL_Rect sourceRect(const Tile tile) {
  switch (tile) {
    case Tile::wall: return {0, 221, 16, 16};
    case Tile::path: return {17, 0, 16, 16};
    case Tile::room: return {0, 0, 16, 16};
    case Tile::closed_door: return {68, 153, 16, 16};
    case Tile::open_door: return {102, 153, 16, 16};
    case Tile::stairs: return {34, 102, 16, 16};
  }
}

}

void renderWorld(
  SDL_Renderer *renderer,
  SDL_Texture *texture,
  const World &world,
  const Light &light
) {
  const gfx::Surface<const Tile> tiles = world.tiles;
  const gfx::Surface<const Visibility> visible = light.visibility;
  for (int y = 0; y != tiles.height(); ++y) {
    for (int x = 0; x != tiles.width(); ++x) {
      setColorMod(texture, visible.ref(x, y));
      const SDL_Rect srcRect = sourceRect(tiles.ref(x, y));
      const SDL_Rect dstRect = {x * 16, y * 16, 16, 16};
      SDL_CHECK(SDL_RenderCopy(renderer, texture, &srcRect, &dstRect));
    }
  }
  SDL_CHECK(SDL_SetTextureColorMod(texture, 255, 255, 255));
}
