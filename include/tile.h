#ifndef _TILE_H_
#define _TILE_H_

#include "sdl_helper.h"

typedef enum {
  TILE_TYPE_NONE = 0,
  TILE_TYPE_BRICK,
  TILE_TYPE_COUNT,
} Tile_type;

typedef struct {
  int collbox_id;
  SDL_Sprite spr;
  Tile_type type;
  SDL_Renderer* ren;
  bool collidable;
} Tile;


int Tile_load_all_tiles(SDL_Renderer* ren);
int Tile_init(Tile* t, const Tile_type type, bool collidable, SDL_Renderer* ren);
void Tile_set_collidable(Tile* t, bool collidable, Tile* tiles);
int Tile_set_type(Tile* t, const Tile_type type);
int Tile_draw(Tile* t, bool debug);

#endif /* _TILE_H_ */
