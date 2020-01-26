//
//  texture loading.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef texture_loading_hpp
#define texture_loading_hpp

struct SDL_Texture;
struct SDL_Renderer;

SDL_Texture *loadTexture(SDL_Renderer *, const char *);

#endif
