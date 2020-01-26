//
//  main.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 24/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include <string>
#include <SDL2/SDL.h>
#include "Game/world.hpp"
#include "Game/light.hpp"
#include "Game/vision.hpp"
#include "Game/sdl check.hpp"
#include "Game/sdl delete.hpp"
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
  
    window.reset(SDL_CHECK(SDL_CreateWindow(
      "Silver Lobster",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      1280, 720,
      SDL_WINDOW_SHOWN
    )));
    
    renderer.reset(SDL_CHECK(SDL_CreateRenderer(
      window.get(),
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    )));
    
    sprites = loadTexture(renderer.get(), res("prototype spritesheet.png").c_str());
  }
  
  void quit() {
    sprites.reset();
    renderer.reset();
    window.reset();
    SDL_Quit();
  }
  
  void render() {
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    renderWorld(renderer.get(), sprites.get(), world, light);
    const SDL_Rect srcRect = {459, 0, 16, 16};
    const SDL_Rect dstRect = {16 * player.x, 16 * player.y, 16, 16};
    SDL_CHECK(SDL_RenderCopy(renderer.get(), sprites.get(), &srcRect, &dstRect));
    SDL_RenderPresent(renderer.get());
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
    updateLight(light, world, player, visParams);
    // illuminate(light);
  }
  
  void input(const SDL_Scancode key) {
    switch (key) {
      case SDL_SCANCODE_UP:
        --player.y;
        break;
      case SDL_SCANCODE_RIGHT:
        ++player.x;
        break;
      case SDL_SCANCODE_DOWN:
        ++player.y;
        break;
      case SDL_SCANCODE_LEFT:
        --player.x;
        break;
      default:
        return;
    }
    
    updateLight(light, world, player, {-1});
    render();
  }
  
  void run() {
    bool running = true;
    do {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          running = false;
        } else if (e.type == SDL_KEYDOWN) {
          input(e.key.keysym.scancode);
        }
      }
      
      SDL_Delay(5);
    } while (running);
  }
  
private:
  SDL::Window window;
  SDL::Renderer renderer;
  SDL::Texture sprites;
  World world;
  Light light;
  gfx::Point player = {12, 1};
};

int main() {
  Game game;
  game.init();
  game.initLevel();
  game.render();
  game.run();
  game.quit();
}
