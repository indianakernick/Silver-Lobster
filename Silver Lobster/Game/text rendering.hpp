//
//  text rendering.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 3/2/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef text_rendering_hpp
#define text_rendering_hpp

struct SDL_Renderer;
struct _TTF_Font;

void renderText(SDL_Renderer *, _TTF_Font *, const char *, int, int);

#endif
