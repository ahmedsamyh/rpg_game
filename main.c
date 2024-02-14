#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sdl_helper.h"
#include <math.h>
#include "stb_image.h"
#include "player.h"
#include "collision.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "globals.h"

SDL_FRect* collboxes = NULL;

// TODO: Check commit 7e9f5fc

int main(int argc, char* argv[]){
  SDL_SetMainReady();
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
    fprintf(stderr, "ERROR: SDL_Init() -> %s\n", SDL_GetError());
    exit(1);
  }

  if (TTF_Init() < 0){
    fprintf(stderr, "ERROR: TTF_Init() -> %s\n", TTF_GetError());
    exit(1);
  }

  TTF_Font *font = TTF_OpenFont("resources/Tektur-VariableFont_wdth,wght.ttf", 16);

  if (font == NULL){
    fprintf(stderr, "ERROR: TTF_OpenFont() -> %s\n", TTF_GetError());
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
  int FPS = 0;

  SDL_FPoint mpos = {0};
  SDL_FPoint unscaled_mpos = {0};

  float scl = 2.f;
  if (SDL_RenderSetScale(ren, scl, scl) < 0){
    fprintf(stderr, "ERROR: SDL_RenderSetScale() -> %s\n", SDL_GetError());
    quit = true;
  }

  float scaled_width = width/scl;
  float scaled_height = height/scl;

  if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND) < 0){
    fprintf(stderr, "ERROR: SDL_SetRenderDrawBlendMode() -> %s\n", SDL_GetError());
    quit = true;
  }

  bool DEBUG_DRAW = false;

  Player p = {0};

  if (Player_init(&p, ren) < 0){
    quit = true;
  }

  p.spr.pos.x = scaled_width*0.5f;
  p.spr.pos.y = scaled_height*0.5f;

  int hovering_box_idx = -1;

  arrput(collboxes, ((SDL_FRect) {
    .x = 100.f,
    .y = 100.f,
    .w = 50.f,
    .h = 100.f
      }));

  while (!quit){

    tp2 = SDL_GetTicks64();
    delta = (tp2 - tp1)/1000.f;
    tp1 = tp2;

    FPS = (int)(1.f / delta);

#define MAX_TITLE_SIZE 1024
    char full_title[MAX_TITLE_SIZE];

    if (sprintf(full_title, "%s | FPS: %d | delta: %f", title, FPS, delta) < 0){
      fprintf(stderr, "ERROR: sprintf() -> %s\n", strerror(errno));
      quit = true;
    }

    SDL_SetWindowTitle(win, full_title);

    SDL_Event e;
    while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT){
	quit = true;
      } else if (e.type == SDL_MOUSEMOTION){
	unscaled_mpos.x = (float)e.motion.x;
	unscaled_mpos.y = (float)e.motion.y;
      } else if (e.type == SDL_KEYDOWN){
	if (e.key.keysym.scancode == SDL_SCANCODE_TILDE){
	  DEBUG_DRAW = !DEBUG_DRAW;
	}
	if (e.key.keysym.scancode == SDL_SCANCODE_Z){
	  for (size_t i = 0; i < 1; ++i){
	    // add 1 new collboxes
	    arrput(collboxes, ((SDL_FRect) {
		  .x = mpos.x,
		  .y = mpos.y,
		  .w = randomf(10.f, 150.f),
		  .h = randomf(10.f, 150.f),
		}));
	  }
	}
	if (e.key.keysym.scancode == SDL_SCANCODE_X){
	  // delete hovering box
	  if (hovering_box_idx >= 0){
	    arrdel(collboxes, hovering_box_idx);
	  }
	}
      }
    }

    // scale mpos
    float sx, sy;
    SDL_RenderGetScale(ren, &sx, &sy);
    mpos.x = unscaled_mpos.x / sx;
    mpos.y = unscaled_mpos.y / sy;

    SDL_RenderClearColorPacked(ren, 0xFF141414);

    Player_update(&p, delta);

    if (Player_draw(&p, DEBUG_DRAW) < 0){
      quit = true;
    }

    bool hovering=false;
    for (size_t i = 0; i < arrlenu(collboxes); ++i){
      SDL_FRect* box = &collboxes[i];
      bool coll = Check_rect_rect_collision(*box, p.hitbox);
      bool mcoll = Check_rect_point_collision(*box, mpos);
      if (mcoll){
	hovering_box_idx = (int)i;
	hovering = true;
      }
      if (SDL_RenderDrawRectFColorPacked(ren, box, coll ? SDL_GREEN : mcoll ? SDL_BLUE : SDL_RED) < 0){
	quit = true;
      }

      if (Resolve_rect_rect_collision(&p.hitbox, *box)){
	Player_adjust_pos_to_hitbox(&p);
      }
    }
    if (!hovering) hovering_box_idx = -1;

    SDL_RenderPresent(ren);
  }

  Player_destroy(&p);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  TTF_CloseFont(font);

  TTF_Quit();
  SDL_Quit();

  return exit_code;
}
