//
//  main.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 24/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include <SDL2/SDL.h>
#include "Game/sdl.hpp"

#include <iostream>
#include "Game/world.hpp"
#include <Graphics/iterator.hpp>
#include "Game/world generation.hpp"

char tileChar(const Tile tile) {
  switch (tile) {
    case Tile::wall:
      return '#';
    case Tile::path:
      return '.';
    case Tile::room:
      return ' ';
    case Tile::closed_door:
      return '*';
    case Tile::open_door:
      return 'O';
    case Tile::stairs:
      return '/';
  }
}

void print(gfx::Surface<const Tile> tiles) {
  for (auto row : range(tiles)) {
    for (const Tile tile : row) {
      std::cout << tileChar(tile);
    }
    std::cout << '\n';
  }
}

int main() {
  const GenParams params = {
    .seed = 12345,
    .roomSizeMin = 3,
    .roomSizeMax = 9,
    .roomDensity = 200,
    .mazeStraightness = 100,
    .connectionRedundancy = 2
  };

  World world;
  initializeWorld(world, 31, 31);
  generateTerrain(world, params);
  print(world.tiles);
 
  /*
  
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
  
  */
}
