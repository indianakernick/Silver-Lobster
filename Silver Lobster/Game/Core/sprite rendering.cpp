//
//  sprite rendering.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include "sprite rendering.hpp"

#include "world.hpp"
#include "sprite.hpp"
#include "position.hpp"
#include "renderer.hpp"
#include "sdl check.hpp"
#include <SDL2/SDL_render.h>
#include <entt/entity/registry.hpp>

void renderSprites(const entt::registry &reg) {
  auto &renderer = reg.ctx<Renderer>();
  gfx::Surface<const Visibility> visible = reg.ctx<Sight>().visibility;
  auto view = reg.view<const Position, const Sprite>();
  view.each([&](auto pos, auto sprite) {
    if (visible.ref(pos.p) != Visibility::visible) return;
    const SDL_Rect srcRect = {sprite.x, sprite.y, 16, 16};
    const SDL_Rect dstRect = {pos.p.x * 16, pos.p.y * 16, 16, 16};
    SDL_CHECK(SDL_RenderCopy(renderer.ren, renderer.tex, &srcRect, &dstRect));
  });
}
