#ifndef _SDL_HELPER_H_
#define _SDL_HELPER_H_

#include <SDL.h>
#include <stdbool.h>

#define PI 3.1415f

// Colors
#define SDL_RED (0xFF0000FF)
#define SDL_GREEN (0xFF00FF00)
#define SDL_BLUE (0xFFFF0000)
#define SDL_BLACK (0xFF000000)
#define SDL_WHITE (0xFFFFFFFF)
#define SDL_BLANK (0x00000000)

// Rendering
#define CIRCLE_POINTS_CAP 30
static SDL_FPoint circle_points[CIRCLE_POINTS_CAP+1] = {0};

int SDL_SetRenderDrawColorPacked(SDL_Renderer* ren, uint32_t color);
int SDL_RenderClearColor(SDL_Renderer* ren, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_RenderClearColorPacked(SDL_Renderer* ren, uint32_t color);
int SDL_RenderDrawRectFColor(SDL_Renderer* ren, const SDL_FRect* rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_RenderDrawRectFColorPacked(SDL_Renderer* ren, const SDL_FRect* rect, uint32_t color);
int SDL_RenderFillRectFColor(SDL_Renderer* ren, const SDL_FRect* rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_RenderFillRectFColorPacked(SDL_Renderer* ren, const SDL_FRect* rect, uint32_t color);
int SDL_RenderCircleF(SDL_Renderer* ren, SDL_FPoint pos, float radius);

// Vector
SDL_FPoint v2f_add(SDL_FPoint v1, SDL_FPoint v2);
float v2f_mag(SDL_FPoint v);
SDL_FPoint v2f_normalize(SDL_FPoint v);
SDL_FPoint v2f_mul(SDL_FPoint v1, SDL_FPoint v2);


// Sprite
typedef struct {
  SDL_FPoint pos;
  SDL_FPoint origin;
  SDL_FPoint size;
  SDL_FPoint scale;
  bool vflip;
  bool hflip;
  float time_per_frame;
  float accumulated;
  float angle;
  size_t hframe, vframe;
  size_t hframes, vframes;
  unsigned char* data;
  SDL_Rect ren_rect;
  SDL_Texture* tex;
} SDL_Sprite;

#define SPR_MIN_SCALE (0.1f)

int SDL_LoadSprite(SDL_Sprite* spr, const char* filename, size_t hframes, size_t vframes, SDL_Renderer* ren);
int SDL_RenderSprite(SDL_Renderer* ren, SDL_Sprite* spr);
void SDL_DestroySprite(SDL_Sprite* spr);
void SDL_SpriteCenterOrigin(SDL_Sprite* spr);
void SDL_SpriteSetScale(SDL_Sprite* spr, SDL_FPoint* scl);
void SDL_SpriteSetHFrame(SDL_Sprite* spr, size_t hframe);
void SDL_SpriteSetVFrame(SDL_Sprite* spr, size_t vframe);
void SDL_SpriteAnimate(SDL_Sprite* spr, float delta);

#endif /* _SDL_HELPER_H_ */
