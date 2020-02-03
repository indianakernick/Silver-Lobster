//
//  main.cpp
//  Silver Lobster
//
//  Created by Indi Kernick on 24/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#include <string>
#include <SDL2/SDL.h>
#include "scope time.hpp"
#include "Game/renderer.hpp"
#include <SDL2_ttf/SDL_ttf.h>
#include "Game/sdl check.hpp"
#include "Game/sdl delete.hpp"
#include "Game/text rendering.hpp"
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
    SCOPE_TIME("Game::init");
  
    SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
    SDL_CHECK(TTF_Init());
    
    // There seems to be a bug in the Metal driver
    // It doesn't like multiple viewports
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  
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
    
    sprites = loadTexture(renderer.get(), res("prototype spritesheet.png").c_str());
    font.reset(SDL_CHECK(TTF_OpenFont(res("prototype font.ttf").c_str(), 16)));
    
    reg.set<Renderer>(sprites.get(), renderer.get());
  }
  
  void quit() {
    SCOPE_TIME("Game::quit");
  
    font.reset();
    sprites.reset();
    renderer.reset();
    window.reset();
    TTF_Quit();
    SDL_Quit();
  }
  
  void render() {
    SCOPE_TIME("Game::render");
    
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    const SDL_Rect gameView = {
      (1280 - 63 * 16) / 2, (720 - 31 * 16) / 2, 63 * 16, 31 * 16
    };
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), &gameView));
    renderGame(reg);
    const SDL_Rect textView = {
      0, 0, 200, 50
    };
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), &textView));
    renderText(renderer.get(), font.get(), "This is a test", 0, 0);
    SDL_RenderPresent(renderer.get());
  }
  
  void initLevel() {
    SCOPE_TIME("Game::initLevel");
  
    initializeWorld(reg, {63, 31});
    initializeLight(reg, {63, 31});
    generateTerrain(reg, 1234);
    spawnEntities(reg, 1234);
  }
  
  void run() {
    SCOPE_TIME("Game::run");
   
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
  SDL::Font font;
  entt::registry reg;
};

int main() {
  {
    SCOPE_TIME("main");

    Game game;
    game.init();
    game.initLevel();
    game.render();
    game.run();
    game.quit();
  }
  
  SCOPE_TIME_PRINT();
}
