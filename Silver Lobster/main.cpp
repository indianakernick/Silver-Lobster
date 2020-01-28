//
//  main.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 24/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include <string>
#include <SDL2/SDL.h>
#include "Game/tags.hpp"
#include "Game/speed.hpp"
#include "Game/sprite.hpp"
#include "Game/renderer.hpp"
#include "Game/position.hpp"
#include "Game/game loop.hpp"
#include "Game/sdl check.hpp"
#include "Game/sdl delete.hpp"
#include "Game/update light.hpp"
#include "Game/handle input.hpp"
#include "Game/generate world.hpp"
#include <entt/entity/registry.hpp>
#include "Game/render entities.hpp"
#include "Game/texture loading.hpp"

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
    entt::entity player = reg.create();
    reg.assign<Player>(player);
    reg.assign<Position>(player, 12, 1);
    reg.assign<Sprite>(player, 459, 0);
    reg.assign<Speed>(player, Speed::max);
  
    initializeWorld(reg, 63, 31);
    initializeLight(reg, 63, 31);
    generateTerrain(reg);
    updateLight(reg);
    // illuminate(reg);
  }
  
  void run() {
    bool running = true;
    do {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          running = false;
        } else if (e.type == SDL_KEYDOWN) {
          handleKeyDown(reg, e.key.keysym.scancode);
        }
      }
      stepGame(reg);
      render();
    } while (running);
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
