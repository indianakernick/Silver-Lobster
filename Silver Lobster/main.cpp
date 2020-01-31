//
//  main.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 24/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include <string>
#include <SDL2/SDL.h>
#include "Game/renderer.hpp"
#include "Game/sdl check.hpp"
#include "Game/sdl delete.hpp"
#include "Game/Core/game loop.hpp"
#include "Game/Factories/lamp.hpp"
#include <entt/entity/registry.hpp>
#include "Game/texture loading.hpp"
#include "Game/Factories/ghost.hpp"
#include "Game/Factories/player.hpp"
#include "Game/Core/field of view.hpp"
#include "Game/Core/world spawning.hpp"
#include "Game/Core/input handling.hpp"
#include "Game/Core/world generation.hpp"

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
      SDL_RENDERER_ACCELERATED
    )));
    
    const SDL_Rect viewport = {
      (1280 - 63 * 16) / 2, (720 - 31 * 16) / 2, 63 * 16, 31 * 16
    };
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), &viewport));
    
    sprites = loadTexture(renderer.get(), res("prototype spritesheet.png").c_str());
    
    reg.set<Renderer>(sprites.get(), renderer.get());
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
    renderGame(reg);
    SDL_RenderPresent(renderer.get());
  }
  
  void initLevel() {
    initializeWorld(reg, {63, 31});
    initializeLight(reg, {63, 31});
    generateTerrain(reg, 1234);
    spawnEntities(reg, 1234);
  }
  
  void run() {
    while (true) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          return;
        } else if (e.type == SDL_KEYDOWN) {
          handleKeyDown(reg, e.key.keysym.scancode);
        }
      }
      
      if (stepGame(reg)) {
        while (stepGame(reg));
        render();
      } else {
        SDL_Delay(10);
      }
    }
  }
  
private:
  SDL::Window window;
  SDL::Renderer renderer;
  SDL::Texture sprites;
  entt::registry reg;
};

int main() {
  Game game;
  game.init();
  game.initLevel();
  game.render();
  game.run();
  game.quit();
}
