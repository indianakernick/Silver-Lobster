//
//  main.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 24/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include <SDL2/SDL.h>
#include "Game/sdl.hpp"
#include <entt/entt.hpp>

int main() {
  SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
  
  SDL_Window *window = SDL_CHECK(SDL_CreateWindow(
    "Silver Lobster",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    1280, 720,
    SDL_WINDOW_SHOWN
  ));
  
  SDL_Renderer *renderer = SDL_CHECK(SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  ));
  
  bool running = true;
  do {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
    }
    
    SDL_CHECK(SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255));
    SDL_CHECK(SDL_RenderClear(renderer));
    SDL_RenderPresent(renderer);
  } while (running);
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
