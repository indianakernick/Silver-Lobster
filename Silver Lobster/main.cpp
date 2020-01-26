//
//  main.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 24/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include <string>
#include <SDL2/SDL.h>
#include "Game/sdl.hpp"
#include "Game/world.hpp"
#include "Game/light.hpp"
#include "Game/vision.hpp"
#include "Game/texture loading.hpp"
#include "Game/world rendering.hpp"
#include "Game/world generation.hpp"

std::string res(const char *path) {
  return std::string(SDL_CHECK(SDL_GetBasePath())) + path;
}

class Game {
public:
  void init() {
    SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
  
    window = SDL_CHECK(SDL_CreateWindow(
      "Silver Lobster",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      1280, 720,
      SDL_WINDOW_SHOWN
    ));
    
    renderer = SDL_CHECK(SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    ));
    
    sprites = loadTexture(renderer, res("prototype spritesheet.png").c_str());
  }
  
  void quit() {
    SDL_DestroyTexture(sprites);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
  
  void render() {
    SDL_CHECK(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer));
    renderWorld(renderer, sprites, world, light);
    SDL_RenderPresent(renderer);
  }
  
  void initLevel() {
    const GenParams genParams = {
      .seed = 12345,
      .roomSizeMin = 3,
      .roomSizeMax = 9,
      .roomDensity = 200,
      .mazeStraightness = 100,
      .connectionRedundancy = 2
    };
    const VisParams visParams = {
      .range = -1
    };
    
    initializeWorld(world, 31, 31);
    initializeLight(light, 31, 31);
    generateTerrain(world, genParams);
    // updateLight(light, world, {12, 1}, visParams);
    illuminate(light);
  }
  
  void run() {
    bool running = true;
    do {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          running = false;
        }
      }
      
      SDL_Delay(5);
    } while (running);
  }
  
private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *sprites = nullptr;
  World world;
  Light light;
};

int main() {
  Game game;
  game.init();
  game.initLevel();
  game.render();
  game.run();
  game.quit();
}
