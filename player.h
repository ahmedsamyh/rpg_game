#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SDL2/SDL.h>
#include "sdl_helper.h"

typedef struct {
  SDL_FPoint dir;
  SDL_FPoint vel;
  float speed;
  SDL_Sprite spr;
  SDL_Renderer* ren;
  SDL_FRect hitbox;

  float hp, max_hp;
  float mp, max_mp;
} Player;

int Player_init(Player* p,  SDL_Texture_wrapper* tex_wrap, SDL_Renderer* ren);
void Player_update(Player* p, float delta);
int Player_draw(Player* p, bool debug);
void Player_control(Player* p);
void Player_animate(Player* p);
void Player_destroy(Player* p);
void Player_adjust_pos_to_hitbox(Player* p);

#endif /* _PLAYER_H_ */
