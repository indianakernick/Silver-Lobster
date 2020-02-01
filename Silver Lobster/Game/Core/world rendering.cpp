//
//  world rendering.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "world rendering.hpp"

#include "world.hpp"
#include "renderer.hpp"
#include "sdl check.hpp"
#include "scope time.hpp"
#include <SDL2/SDL_render.h>
#include <entt/entity/registry.hpp>

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

void renderWorld(const entt::registry &reg) {
  SCOPE_TIME("renderWorld");

  const gfx::Surface<const Tile> tiles = reg.ctx<World>().tiles;
  const gfx::Surface<const Visibility> visible = reg.ctx<Sight>().visibility;
  auto renderer = reg.ctx<Renderer>();
  for (int y = 0; y != tiles.height(); ++y) {
    for (int x = 0; x != tiles.width(); ++x) {
      if (visible.ref(x, y) == Visibility::unexplored) continue;
      setColorMod(renderer.tex, visible.ref(x, y));
      const SDL_Rect srcRect = sourceRect(tiles.ref(x, y));
      const SDL_Rect dstRect = {x * 16, y * 16, 16, 16};
      SDL_CHECK(SDL_RenderCopy(renderer.ren, renderer.tex, &srcRect, &dstRect));
    }
  }
  SDL_CHECK(SDL_SetTextureColorMod(renderer.tex, 255, 255, 255));
}
