//
//  renderer.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef renderer_hpp
#define renderer_hpp

struct SDL_Texture;
struct SDL_Renderer;

struct Renderer {
  SDL_Texture *tex;
  SDL_Renderer *ren;
};

#endif
