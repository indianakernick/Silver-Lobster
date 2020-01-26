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

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;

namespace SDL {

struct DeleteWindow {
  void operator()(SDL_Window *) const noexcept;
};

struct DeleteTexture {
  void operator()(SDL_Texture *) const noexcept;
};

struct DeleteRenderer {
  void operator()(SDL_Renderer *) const noexcept;
};

using Window = std::unique_ptr<SDL_Window, DeleteWindow>;
using Texture = std::unique_ptr<SDL_Texture, DeleteTexture>;
using Renderer = std::unique_ptr<SDL_Renderer, DeleteRenderer>;

}

#endif
