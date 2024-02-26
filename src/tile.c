#include "tile.h"
#include <assert.h>
#include "globals.h"
#include "config.h"
#include "stb_ds.h"

int Tile_load_all_tiles(SDL_Renderer* ren){
  for (size_t i = 0; i < (size_t)TILE_TYPE_COUNT; ++i){
    snprintf(tmp_buff, TMP_BUFF_CAP, "resources/tile%zu.png", i);
    if (SDL_LoadTexture(ren, tmp_buff) == NULL){
      return -1;
    }
  }
  return 0;
}

int Tile_init(Tile* t, const Tile_type type, bool collidable, SDL_Renderer* ren){
  Tile_set_collidable(t, collidable, NULL);
  t->ren = ren;

  return Tile_set_type(t, type);
}

void Tile_set_collidable(Tile* t, bool collidable, Tile* tiles){
  bool was_collidable = t->collidable;
  t->collidable = collidable;
  if (collidable){
    t->collbox_id = arrlenu(collboxes);
    arrput(collboxes, ((SDL_FRect){
	  .x = t->spr.pos.x,
	  .y = t->spr.pos.y,
	  .w = (float)TILE_SIZE,
	  .h = (float)TILE_SIZE,
	}));
  } else {
    if (was_collidable){
      // push the collbox_id's right to this one left
      for (size_t i = 0 ; i < arrlenu(tiles); ++i){
	if (tiles[i].collbox_id > t->collbox_id){
	  SDL_assert(tiles[i].collbox_id > 0);
	  tiles[i].collbox_id = tiles[i].collbox_id - 1;
	}
      }
      arrdel(collboxes, t->collbox_id);
    }
    t->collbox_id = -1;
  }
}

int Tile_set_type(Tile* t, const Tile_type type){
  SDL_assert((int)t->type < (int)TILE_TYPE_COUNT);
  t->type = type;
  snprintf(tmp_buff, TMP_BUFF_CAP, "resources/tile%d.png", (int)t->type);
  SDL_Sprite* tmp = (SDL_Sprite*)malloc(sizeof(SDL_Sprite));
  memcpy(tmp, &t->spr, sizeof(SDL_Sprite));
  if (SDL_LoadSprite(&t->spr, SDL_LoadTexture(t->ren, tmp_buff), 1, 1, t->ren) < 0){
    return -1;
  }
  t->spr.pos = tmp->pos;

  free(tmp);
  return 0;
}

int Tile_draw(Tile* t, bool debug){
  if (SDL_RenderSprite(t->ren, &t->spr) < 0){
    return -1;
  }

  if (debug){
    if (t->collidable){
      if (!(0 <= t->collbox_id && t->collbox_id < arrlen(collboxes))){
	SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "t->collbox_id: %zu", t->collbox_id);
	SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "arrlen(collboxes): %zu", arrlen(collboxes));
	SDL_assert(0);
      }
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
