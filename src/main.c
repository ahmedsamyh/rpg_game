#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sdl_helper.h>
#include <math.h>
#include <stb_image.h>
#include <player.h>
#include <collision.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include <globals.h>
#include <tile.h>
#include <config.h>
#include "youko-sheet.c"

SDL_FRect* collboxes = NULL;
SDL_Texture_wrapper_KV* texture_map;
int screen_width = 1280;
int screen_height = 720;
float scl = 2.f;
float width  = 0.f;
float height = 0.f;

// TODO: Check commit 0237f99f16e112d7f510bf98ce9d3a235a23aa4d

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

  const char* title = "RPG Game";

  SDL_Window* win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, 0);

  if (win == NULL){
    fprintf(stderr, "ERROR: Could not create window: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  width  = screen_width/scl;
  height = screen_height/scl;

  bool quit = false;
  int exit_code = 0;

  float tp1 = SDL_GetTicks64();
  float tp2 = SDL_GetTicks64();
  float delta = tp2 - tp1;
  int FPS = 0;

  Mouse m = {0};
  Mouse_init(&m, ren);
  SDL_FPoint cursor = {0};

  if (SDL_RenderSetScale(ren, scl, scl) < 0){
    fprintf(stderr, "ERROR: SDL_RenderSetScale() -> %s\n", SDL_GetError());
    quit = true;
  }


  if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND) < 0){
    fprintf(stderr, "ERROR: SDL_SetRenderDrawBlendMode() -> %s\n", SDL_GetError());
    quit = true;
  }

  bool DEBUG_DRAW = false;

  Player p = {0};

  if (SDL_LoadTextureFromMemory(ren, youko_sheet_bytes, youko_sheet_size, "resources/youko-sheet.png") == NULL) { quit = true; }
  if (Tile_load_all_tiles(ren) < 0) { quit = true; }

  if (Player_init(&p, SDL_LoadTexture(ren, "resources/youko-sheet.png"), ren) < 0){
    quit = true;
  }

  p.spr.pos.x = width*0.5f;
  p.spr.pos.y = height*0.5f;

  int hovering_box_idx = -1;

  Tile* tiles = NULL;

  const size_t rows = (int)screen_width / TILE_SIZE;
  const size_t cols = (int)screen_height / TILE_SIZE;

  for (size_t y = 0; y < rows; ++y){
    for (size_t x = 0; x < cols; ++x){
      Tile t = {0};

      if (Tile_init(&t, TILE_TYPE_NONE, false, ren) < 0){
	quit = true;
      }

      t.spr.pos.x = x*TILE_SIZE;
      t.spr.pos.y = y*TILE_SIZE;

      arrput(tiles, t);
    }
  }

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
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT){
	quit = true;
      } else if (e.type == SDL_MOUSEMOTION){
	m.unscaled_pos.x = (float)e.motion.x;
	m.unscaled_pos.y = (float)e.motion.y;
      } else if (e.type == SDL_KEYDOWN){
	if (e.key.keysym.scancode == SDL_SCANCODE_TILDE){
	  DEBUG_DRAW = !DEBUG_DRAW;
	}
      }
    }

    Mouse_button_update(&m);

    cursor.x = (float)(((int)m.pos.x / (int)TILE_SIZE)*TILE_SIZE);
    cursor.y = (float)(((int)m.pos.y / (int)TILE_SIZE)*TILE_SIZE);

    SDL_RenderClearColorPacked(ren, 0xFF141414);

    Player_update(&p, delta);

    // toggle-collision of tile on mouse click

    if (m.pressed[MB_LEFT]){
      printf("HEL\n");
    }

    for (size_t i = 0; i < arrlenu(tiles); ++i){
      Tile t = tiles[i];
      if (Tile_draw(&t, DEBUG_DRAW) < 0){
	quit = true;
      }
    }

    if (Player_draw(&p, DEBUG_DRAW) < 0){
      quit = true;
    }

    if (Player_draw_ui(&p, DEBUG_DRAW) < 0){
      quit = true;
    }

    bool hovering=false;
    for (size_t i = 0; i < arrlenu(collboxes); ++i){
      SDL_FRect* box = &collboxes[i];
      bool coll = Check_rect_rect_collision(*box, p.hitbox);
      bool mcoll = Check_rect_point_collision(*box, m.pos);
      if (mcoll){
	hovering_box_idx = (int)i;
	hovering = true;
      }
      /* if (SDL_RenderDrawRectFColorPacked(ren, box, coll ? SDL_GREEN : mcoll ? SDL_BLUE : SDL_RED) < 0){ */
      /* 	quit = true; */
      /* } */

      if (Resolve_rect_rect_collision(&p.hitbox, *box)){
	Player_adjust_pos_to_hitbox(&p);
      }
    }
    if (!hovering) hovering_box_idx = -1;

    SDL_RenderPresent(ren);
  }

  Player_destroy(&p);
  arrfree(collboxes);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  TTF_CloseFont(font);

  TTF_Quit();
  SDL_Quit();

  return exit_code;
}
