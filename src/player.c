#include "player.h"
#include <stdio.h>
#include <assert.h>
#include "stb_ds.h"
#include "collision.h"
#include "globals.h"

int Player_init(Player* p,  SDL_Texture_wrapper* tex_wrap, SDL_Renderer* ren){
  if (SDL_LoadSprite(&p->spr, tex_wrap, 2, 4, ren) < 0){
    return -1;
  }
  SDL_SpriteCenterOrigin(&p->spr);

  // scale up player sprite for now because it's 16x16, we want it as 64x64
  const float actual_size = 32.f;
  float s = actual_size / p->spr.ren_rect.w;
  SDL_FPoint scl = {s, s};

  SDL_SpriteSetScale(&p->spr, &scl);

  p->ren = ren;
  p->dir = (SDL_FPoint){0.f, 0.f};
  p->vel = (SDL_FPoint){0.f, 0.f};
  p->speed = 200.f;
  p->hitbox = (SDL_FRect){0.f, 0.f, actual_size, actual_size};
  p->max_hp = 30.f;
  p->max_mp = 10.f;
  p->hp = p->max_hp;
  p->mp = p->max_mp;

  return 0;
}

void Player_update(Player* p, float delta){
  // control player
  Player_control(p);

  // update player pos by the vel
  p->vel = v2f_mul_scalar(v2f_add(p->vel, v2f_mul_scalar(p->dir, p->speed)), delta);
  SDL_FPoint next_pos = v2f_add(p->spr.pos, p->vel);

  SDL_FRect next_hitbox = {
    .x = next_pos.x - p->hitbox.w * 0.5f,
    .y = next_pos.y - p->hitbox.h * 0.5f,
    .w = p->hitbox.w,
    .h = p->hitbox.h,
  };

  // check for collisions against all of the collboxes
  Coll_Axis caxis = COLL_AXIS_NONE;
  int closest_collbox_idx = -1;
  for (size_t i = 0; i < arrlenu(collboxes); ++i){
    if (Check_rect_rect_collision_ex(next_hitbox, collboxes[i], &caxis)){
      closest_collbox_idx = (int)i;
      break;
    }
  }

  if (caxis != COLL_AXIS_VERTICAL){
    p->spr.pos.y = next_pos.y;
  } else {
    assert(closest_collbox_idx >= 0);
    SDL_FRect hit_collbox = collboxes[closest_collbox_idx];
    if (p->spr.pos.y < hit_collbox.y)
      p->spr.pos.y = hit_collbox.y - p->hitbox.h * 0.5f;
    else
      p->spr.pos.y = hit_collbox.y + hit_collbox.h + p->hitbox.h * 0.5f;
  }
  if (caxis != COLL_AXIS_HORIZONTAL){
    p->spr.pos.x = next_pos.x;
  } else {
    assert(closest_collbox_idx >= 0);
    SDL_FRect hit_collbox = collboxes[closest_collbox_idx];
    if (p->spr.pos.x < hit_collbox.x)
      p->spr.pos.x = hit_collbox.x - p->hitbox.w * 0.5f;
    else
      p->spr.pos.x = hit_collbox.x + hit_collbox.w + p->hitbox.w * 0.5f;
  }

  if (caxis != COLL_AXIS_NONE){
    p->hp -= 0.1f;
    if (p->hp < 0.f) p->hp = 0.f;
  }

  // update hitbox pos
  p->hitbox.x = p->spr.pos.x - p->hitbox.w * 0.5f;
  p->hitbox.y = p->spr.pos.y - p->hitbox.h * 0.5f;

  // animate if moving
  if (v2f_mag2(p->dir) > 0.f)
    SDL_SpriteAnimate(&p->spr, delta);
}

int Player_draw(Player* p, bool debug){
  Player_animate(p);
  if (SDL_RenderSprite(p->ren, &p->spr) < 0){
    return -1;
  }

  if (debug){
    SDL_RenderDrawRectFColorPacked(p->ren, &p->hitbox, SDL_RED);

    // draw hp bar
    float padding = 10.f;
    float bar_height = 3.f;
    float bar_width = p->hitbox.w;
    const SDL_FRect mp_rect = {
      .x = p->hitbox.x,
      .y = p->hitbox.y - bar_height - padding,
      .w = (p->mp / p->max_mp)*bar_width,
      .h = bar_height,
    };
    SDL_RenderFillRectFColorPacked(p->ren, &mp_rect, SDL_BLUE);

    SDL_FRect hp_rect = mp_rect;
    hp_rect.w = (p->hp / p->max_hp)*bar_width;
    hp_rect.y -= padding;
    SDL_RenderFillRectFColorPacked(p->ren, &hp_rect, SDL_GREEN);
  }

  return 0;
}

void Player_control(Player* p){
  p->dir = (SDL_FPoint){0.f, 0.f};
  if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]){
    p->dir.x--;
  }
  if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]){
    p->dir.x++;
  }
  if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W]){
    p->dir.y--;
  }
  if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]){
    p->dir.y++;
  }

  p->dir = v2f_normalize(p->dir);
}

void Player_animate(Player* p){
  if (p->dir.x < 0.f) { // left
    SDL_SpriteSetVFrame(&p->spr, 2);
  } else if (p->dir.x > 0.f) { // right
    SDL_SpriteSetVFrame(&p->spr, 1);
  }

  if (p->dir.y < 0.f) { // up
    SDL_SpriteSetVFrame(&p->spr, 3);
  } else if (p->dir.y > 0.f) { // down
    SDL_SpriteSetVFrame(&p->spr, 0);
  }
}

void Player_destroy(Player* p){

}

void Player_adjust_pos_to_hitbox(Player* p){
  p->spr.pos.x = p->hitbox.x + p->hitbox.w * 0.5f;
  p->spr.pos.y = p->hitbox.y + p->hitbox.h * 0.5f;

}

int Player_draw_ui(Player* p, bool debug){
  if (debug){
    // draw icon
    const SDL_FRect icon_rect = {
      .x = (float)PLAYER_ICON_PADDING,
      .y = (float)height - (float)PLAYER_ICON_SIZE - (float)PLAYER_ICON_PADDING,
      .w = (float)PLAYER_ICON_SIZE,
      .h = (float)PLAYER_ICON_SIZE,
    };
    if (SDL_RenderFillRectFColorPacked(p->ren, &icon_rect, 0xFF1414FF) < 0)
      return -1;

    // draw health bar
    SDL_FRect hp_rect = {
      .x = icon_rect.x + icon_rect.w + (float)PLAYER_HP_BAR_PADDING,
      .y = icon_rect.y,
      .w = (p->hp / p->max_hp)*PLAYER_HP_BAR_WIDTH,
      .h = (float)PLAYER_HP_BAR_HEIGHT,
    };

    if (SDL_RenderFillRectFColorPacked(p->ren, &hp_rect, SDL_GREEN) < 0)
      return -1;
    hp_rect.w = PLAYER_HP_BAR_WIDTH;
    if (SDL_RenderDrawRectFColorPacked(p->ren, &hp_rect, SDL_WHITE) < 0)
      return -1;

    // draw magic bar
    SDL_FRect mp_rect = {
      .x = icon_rect.x + icon_rect.w + (float)PLAYER_MP_BAR_PADDING,
      .y = icon_rect.y + hp_rect.h + (float)PLAYER_HP_BAR_PADDING,
      .w = (p->mp / p->max_mp)*PLAYER_MP_BAR_WIDTH,
      .h = (float)PLAYER_MP_BAR_HEIGHT,
    };

    if (SDL_RenderFillRectFColorPacked(p->ren, &mp_rect, SDL_BLUE) < 0)
      return -1;
    mp_rect.w = PLAYER_MP_BAR_WIDTH;
    if (SDL_RenderDrawRectFColorPacked(p->ren, &mp_rect, SDL_WHITE) < 0)
      return -1;
  }

  return 0;
}
