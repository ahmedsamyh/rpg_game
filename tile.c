#include "tile.h"
#include <assert.h>
#include "globals.h"
#include "config.h"
#include "stb_ds.h"

int Tile_load_all_tiles(SDL_Renderer* ren){
  for (size_t i = 0; i < (size_t)TILE_TYPE_COUNT; ++i){
    snprintf(tmp_buff, TMP_BUFF_CAP, "resources/tile%zu.png", i);
    if (SDL_TextureLoad(ren, tmp_buff) == NULL){
      return -1;
    }
  }
  return 0;
}

int Tile_init(Tile* t, const Tile_type type, bool collidable, SDL_Renderer* ren){
  t->collidable = collidable;
  if (collidable){
    t->collbox_id = arrlenu(collboxes);
    arrput(collboxes, ((SDL_FRect){
	  .x = 0.f,
	  .y = 0.f,
	  .w = (float)TILE_SIZE,
	  .h = (float)TILE_SIZE,
	}));
  } else {
    t->collbox_id = -1;
  }
  t->type = type;
  t->ren = ren;

  assert((int)t->type < (int)TILE_TYPE_COUNT);

  snprintf(tmp_buff, TMP_BUFF_CAP, "resources/tile%d.png", (int)t->type);
  return SDL_LoadSprite(&t->spr, SDL_TextureLoad(ren, tmp_buff), 1, 1, t->ren);
}


int Tile_draw(Tile* t, bool debug){
  if (SDL_RenderSprite(t->ren, &t->spr) < 0){
    return -1;
  }

  if (debug){
    if (t->collidable){
      assert(0 >= t->collbox_id && t->collbox_id <= arrlenu(collboxes)-1);
      SDL_FRect* rect = &collboxes[t->collbox_id];
      if (SDL_RenderDrawRectFColorPacked(t->ren, rect, SDL_RED) < 0 ){
	return -1;
      }
    } else {
      const SDL_FRect rect = {
	.x = t->spr.pos.x,
	.y = t->spr.pos.y,
	.w = (float)TILE_SIZE,
	.h = (float)TILE_SIZE,
      };
      if (SDL_RenderDrawRectFColorPacked(t->ren, &rect, SDL_BLUE) < 0 ){
	return -1;
      }
    }
  }
  return 0;
}
