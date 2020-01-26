//
//  world rendering.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef world_rendering_hpp
#define world_rendering_hpp

struct World;
struct Light;
struct SDL_Texture;
struct SDL_Renderer;

void renderWorld(SDL_Renderer *, SDL_Texture *, const World &, const Light &);

#endif
