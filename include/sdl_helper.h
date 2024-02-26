#ifndef _SDL_HELPER_H_
#define _SDL_HELPER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "SDL_Texture_wrapper.h"

#define PI 3.1415f

// Mouse
enum {
  MB_LEFT,
  MB_RIGHT,
  MB_MIDDLE,
  MB_COUNT,
};

typedef struct {
  bool released[(int)MB_COUNT];
  bool pressed[(int)MB_COUNT];
  bool held[(int)MB_COUNT];
  SDL_FPoint pos;
  SDL_FPoint unscaled_pos;
  SDL_Renderer* ren;
} Mouse;

void Mouse_init(Mouse* m, SDL_Renderer* ren);
void Mouse_button_update(Mouse* m);

// Tmp buffer
#define TMP_BUFF_CAP (1024*32)
static char tmp_buff[TMP_BUFF_CAP] = {0};

// tilde key is called grave in sdl
#define SDL_SCANCODE_TILDE SDL_SCANCODE_GRAVE

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

static SDL_Surface* text_surface = NULL;

int SDL_SetRenderDrawColorPacked(SDL_Renderer* ren, uint32_t color);
int SDL_RenderClearColor(SDL_Renderer* ren, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_RenderClearColorPacked(SDL_Renderer* ren, uint32_t color);
int SDL_RenderDrawRectFColor(SDL_Renderer* ren, const SDL_FRect* rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_RenderDrawRectFColorPacked(SDL_Renderer* ren, const SDL_FRect* rect, uint32_t color);
int SDL_RenderFillRectFColor(SDL_Renderer* ren, const SDL_FRect* rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_RenderFillRectFColorPacked(SDL_Renderer* ren, const SDL_FRect* rect, uint32_t color);
int SDL_RenderCircleF(SDL_Renderer* ren, SDL_FPoint pos, float radius);
int SDL_RenderCircleFColor(SDL_Renderer* ren, SDL_FPoint pos, float radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_RenderCircleFColorPacked(SDL_Renderer* ren, SDL_FPoint pos, float radius, uint32_t color);
int TTF_RenderText_BlendedColor(SDL_Renderer* ren, TTF_Font* font, SDL_FPoint pos, const char* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int TTF_RenderText_BlendedColorPacked(SDL_Renderer* ren, TTF_Font* font, SDL_FPoint pos, const char* text, uint32_t color);

// Vector
SDL_FPoint v2f_add(SDL_FPoint v1, SDL_FPoint v2);
SDL_FPoint v2f_sub(SDL_FPoint v1, SDL_FPoint v2);
float v2f_mag(SDL_FPoint v);
float v2f_mag2(SDL_FPoint v);
SDL_FPoint v2f_normalize(SDL_FPoint v);
SDL_FPoint v2f_mul(SDL_FPoint v1, SDL_FPoint v2);
SDL_FPoint v2f_mul_scalar(SDL_FPoint v1, float s);

// Texture
SDL_Texture_wrapper* SDL_LoadTexture(SDL_Renderer* ren, const char* filename); // default from file
SDL_Texture_wrapper* SDL_LoadTextureFromFile(SDL_Renderer* ren, const char* filename);
SDL_Texture_wrapper* SDL_LoadTextureFromMemory(SDL_Renderer* ren, unsigned char* data, size_t data_size, const char* filename);

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
  SDL_Rect ren_rect;
  SDL_Texture_wrapper* tex_wrap;
} SDL_Sprite;

#define SPR_MIN_SCALE (0.1f)

int SDL_LoadSprite(SDL_Sprite* spr, SDL_Texture_wrapper* tex_wrap, size_t hframes, size_t vframes, SDL_Renderer* ren);
int SDL_RenderSprite(SDL_Renderer* ren, SDL_Sprite* spr);
void SDL_SpriteCenterOrigin(SDL_Sprite* spr);
void SDL_SpriteSetScale(SDL_Sprite* spr, SDL_FPoint* scl);
void SDL_SpriteSetHFrame(SDL_Sprite* spr, size_t hframe);
void SDL_SpriteSetVFrame(SDL_Sprite* spr, size_t vframe);
void SDL_SpriteAnimate(SDL_Sprite* spr, float delta);

// Math
float randomf(float start, float end);

#endif /* _SDL_HELPER_H_ */
