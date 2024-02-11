#include <stdio.h>
#include <stdbool.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "sdl_helper.h"
#include <math.h>
#include "stb_image.h"

int main(void){
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
    fprintf(stderr, "ERROR: SDL_Init() -> %s\n", SDL_GetError());
    exit(1);
  }

  const int width = 1280;
  const int height = 720;

  const char* title = "RPG Game";

  SDL_Window* win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

  if (win == NULL){
    fprintf(stderr, "ERROR: Could not create window: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  bool quit = false;
  int exit_code = 0;

  float tp1 = SDL_GetTicks64();
  float tp2 = SDL_GetTicks64();
  float delta = tp2 - tp1;

  SDL_FPoint mpos = {0};
  SDL_FPoint unscaled_mpos = {0};

  SDL_Sprite spr = {0};

  if (SDL_LoadSprite(&spr, "resources/khu_sheet.png", 3, 1, ren) < 0){
    quit = true;
  }

  SDL_SpriteCenterOrigin(&spr);

  SDL_FPoint scale = {1.f, 1.f};

  if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND) < 0){
    fprintf(stderr, "ERROR: SDL_SetRenderDrawBlendMode() -> %s\n", SDL_GetError());
    quit = true;
  }

  while (!quit){

    tp2 = SDL_GetTicks64();
    delta = (tp2 - tp1)/1000.f;
    tp1 = tp2;

    SDL_Event e;
    while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT){
	quit = true;
      } else if (e.type == SDL_MOUSEMOTION){
	unscaled_mpos.x = (float)e.motion.x;
	unscaled_mpos.y = (float)e.motion.y;
      } else if (e.type == SDL_KEYDOWN){
	if (e.key.keysym.scancode == SDL_SCANCODE_SPACE){
	  spr.vflip = !spr.vflip;
	}

	if (e.key.keysym.scancode == SDL_SCANCODE_A){
	  spr.hflip = !spr.hflip;
	}
      }
    }

    // scale mpos
    float sx, sy;
    SDL_RenderGetScale(ren, &sx, &sy);
    mpos.x = unscaled_mpos.x / sx;
    mpos.y = unscaled_mpos.y / sy;

    SDL_RenderClearColorPacked(ren, 0xFF141414);

    spr.pos = mpos;

    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_X]){
      spr.angle += 50.f * delta;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Z]){
      spr.angle -= 50.f * delta;
    }

    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_C]){
      scale.x -= 10.f * delta;
      scale.y -= 10.f * delta;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_V]){
      scale.x += 10.f * delta;
      scale.y += 10.f * delta;
    }

    SDL_SpriteSetScale(&spr, &scale);

    /* if (SDL_RenderSetScale(ren, scale.x, scale.y) < 0){ */
    /*   fprintf(stderr, "ERROR: SDL_RenderSetScale() -> %s\n", SDL_GetError()); */
    /*   quit = true; */
    /* } */

    SDL_SpriteAnimate(&spr, delta);

    if (SDL_RenderSprite(ren, &spr) < 0){
      quit = true;
    }

    SDL_RenderPresent(ren);
  }

  SDL_DestroySprite(&spr);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  SDL_Quit();

  return exit_code;
}
