//
//  render entities.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "render entities.hpp"

#include "sprite.hpp"
#include "position.hpp"
#include "renderer.hpp"
#include "sdl check.hpp"
#include <SDL2/SDL_render.h>
#include <entt/entity/registry.hpp>

void renderEntities(const entt::registry &reg) {
  auto &renderer = reg.ctx<Renderer>();
  auto view = reg.view<const Position, const Sprite>();
  view.each([&](auto pos, auto sprite) {
    const SDL_Rect srcRect = {sprite.x, sprite.y, 16, 16};
    const SDL_Rect dstRect = {pos.p.x * 16, pos.p.y * 16, 16, 16};
    SDL_CHECK(SDL_RenderCopy(renderer.ren, renderer.tex, &srcRect, &dstRect));
  });
}
