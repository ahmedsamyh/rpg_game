#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SDL2/SDL.h>
#include "sdl_helper.h"

#define PLAYER_ICON_SIZE 64
#define PLAYER_ICON_PADDING 8
#define PLAYER_HP_BAR_WIDTH 128
#define PLAYER_HP_BAR_HEIGHT 8
#define PLAYER_HP_BAR_PADDING 4
#define PLAYER_MP_BAR_WIDTH (PLAYER_HP_BAR_WIDTH)
#define PLAYER_MP_BAR_HEIGHT (PLAYER_HP_BAR_HEIGHT)
#define PLAYER_MP_BAR_PADDING (PLAYER_HP_BAR_PADDING)



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
int Player_draw_ui(Player* p, bool debug);

#endif /* _PLAYER_H_ */
