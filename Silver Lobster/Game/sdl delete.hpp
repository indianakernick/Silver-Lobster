//
//  sdl delete.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 26/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef sdl_delete_hpp
#define sdl_delete_hpp

#include <memory>

struct _TTF_Font;
struct SDL_Window;
struct SDL_Texture;
struct SDL_Surface;
struct SDL_Renderer;

namespace SDL {

struct DeleteFont {
  void operator()(_TTF_Font *) const noexcept;
};

struct DeleteWindow {
  void operator()(SDL_Window *) const noexcept;
};

struct DeleteTexture {
  void operator()(SDL_Texture *) const noexcept;
};

struct DeleteSurface {
  void operator()(SDL_Surface *) const noexcept;
};

struct DeleteRenderer {
  void operator()(SDL_Renderer *) const noexcept;
};

using Font = std::unique_ptr<_TTF_Font, DeleteFont>;
using Window = std::unique_ptr<SDL_Window, DeleteWindow>;
using Texture = std::unique_ptr<SDL_Texture, DeleteTexture>;
using Surface = std::unique_ptr<SDL_Surface, DeleteSurface>;
using Renderer = std::unique_ptr<SDL_Renderer, DeleteRenderer>;

}

#endif
