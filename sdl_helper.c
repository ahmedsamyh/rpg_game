#include "sdl_helper.h"
#include <stdio.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Rendering
int SDL_SetRenderDrawColorPacked(SDL_Renderer* ren, uint32_t color){
  Uint8 r = (uint8_t)((color >> (8*0)) & 0xFF);
  Uint8 g = (uint8_t)((color >> (8*1)) & 0xFF);
  Uint8 b = (uint8_t)((color >> (8*2)) & 0xFF);
  Uint8 a = (uint8_t)((color >> (8*3)) & 0xFF);

  if (SDL_SetRenderDrawColor(ren, r, g, b, a) < 0){
    fprintf(stderr, "SDL_ERROR: %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}

int SDL_RenderClearColor(SDL_Renderer* ren, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  SDL_SetRenderDrawColor(ren, r, g, b, a);
  if (SDL_RenderClear(ren) < 0){
    fprintf(stderr, "ERROR: SDL_RenderClear() -> %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}

int SDL_RenderClearColorPacked(SDL_Renderer* ren, uint32_t color){
  SDL_SetRenderDrawColorPacked(ren, color);
  if (SDL_RenderClear(ren) < 0){
    fprintf(stderr, "ERROR: SDL_RenderClear() -> %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}

int SDL_RenderCircleF(SDL_Renderer* ren, SDL_FPoint pos, float radius){
  for (size_t i = 0; i < CIRCLE_POINTS_CAP; ++i){
    float a = ((float)i/(float)CIRCLE_POINTS_CAP)*2*PI;
    circle_points[i].x = pos.x + sinf(a) * radius;
    circle_points[i].y = pos.y + cosf(a) * radius;
  }

  circle_points[CIRCLE_POINTS_CAP].x = circle_points[0].x;
  circle_points[CIRCLE_POINTS_CAP].y = circle_points[0].y;

  if (SDL_RenderDrawLinesF(ren, circle_points, CIRCLE_POINTS_CAP+1) < 0){
    fprintf(stderr, "ERROR: SDL_RenderDrawLinesF() -> %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}

int SDL_RenderDrawRectFColor(SDL_Renderer* ren, const SDL_FRect* rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  if (SDL_SetRenderDrawColor(ren, r, g, b, a) < 0){
    return -1;
  }

  if (SDL_RenderDrawRectF(ren, rect) < 0){
    fprintf(stderr, "ERROR: SDL_RenderDrawRect() -> %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}

int SDL_RenderDrawRectFColorPacked(SDL_Renderer* ren, const SDL_FRect* rect, uint32_t color){
    if (SDL_SetRenderDrawColorPacked(ren, color) < 0){
    return -1;
  }

  if (SDL_RenderDrawRectF(ren, rect) < 0){
    fprintf(stderr, "ERROR: SDL_RenderDrawRect() -> %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}

int SDL_RenderFillRectFColor(SDL_Renderer* ren, const SDL_FRect* rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  if (SDL_SetRenderDrawColor(ren, r, g, b, a) < 0){
    return -1;
  }

  if (SDL_RenderFillRectF(ren, rect) < 0){
    fprintf(stderr, "ERROR: SDL_RenderDrawRect() -> %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}
int SDL_RenderFillRectFColorPacked(SDL_Renderer* ren, const SDL_FRect* rect, uint32_t color){
    if (SDL_SetRenderDrawColorPacked(ren, color) < 0){
    return -1;
  }

  if (SDL_RenderFillRectF(ren, rect) < 0){
    fprintf(stderr, "ERROR: SDL_RenderDrawRect() -> %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}

int TTF_RenderText_BlendedColor(SDL_Renderer* ren, TTF_Font* font, SDL_FPoint pos, const char* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  SDL_Color color = {
  .r = r,
  .g = g,
  .b = b,
  .a = a
};
  text_surface = TTF_RenderText_Blended(font, text, color);
  if (text_surface == NULL){
    fprintf(stderr, "ERROR: TTF_RenderText_Solid() -> %s\n", TTF_GetError());
    return -1;
  }

  SDL_Texture *text_tex = SDL_CreateTextureFromSurface(ren, text_surface);

  const SDL_FRect tex_rect = {
    .x = pos.x,
      .y = pos.y,
      .w = (float)text_surface->clip_rect.w,
      .h = (float)text_surface->clip_rect.h,
  };

  if (SDL_RenderCopyF(ren, text_tex, NULL, &tex_rect) < 0){
    fprintf(stderr, "ERROR: SDL_RenderCopyF() -> %s\n", SDL_GetError());
    return -1;
  }

  SDL_FreeSurface(text_surface);
  return 0;
}

int TTF_RenderText_BlendedColorPacked(SDL_Renderer* ren, TTF_Font* font, SDL_FPoint pos, const char* text, uint32_t color){
  text_surface = TTF_RenderText_Blended(font, text, *(SDL_Color*)&color);
  if (text_surface == NULL){
    fprintf(stderr, "ERROR: TTF_RenderText_Solid() -> %s\n", TTF_GetError());
    return -1;
  }

  SDL_Texture *text_tex = SDL_CreateTextureFromSurface(ren, text_surface);

  const SDL_FRect tex_rect = {
    .x = pos.x,
    .y = pos.y,
    .w = (float)text_surface->clip_rect.w,
    .h = (float)text_surface->clip_rect.h,
  };

  if (SDL_RenderCopyF(ren, text_tex, NULL, &tex_rect) < 0){
    fprintf(stderr, "ERROR: SDL_RenderCopyF() -> %s\n", SDL_GetError());
    return -1;
  }

  SDL_FreeSurface(text_surface);
  return 0;
}

int TTF_RenderTextF_BlendedColor(SDL_Renderer* ren, TTF_Font* font, SDL_FPoint pos, const char* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  return 0;
}

int TTF_RenderTextF_BlendedColorPacked(SDL_Renderer* ren, TTF_Font* font, SDL_FPoint pos, const char* text, uint32_t color){

  return 0;
}


// Vector
SDL_FPoint v2f_add(SDL_FPoint v1, SDL_FPoint v2){
  return (SDL_FPoint){
    .x = v1.x + v2.x,
    .y = v1.y + v2.y,
  };
}

SDL_FPoint v2f_sub(SDL_FPoint v1, SDL_FPoint v2){
    return (SDL_FPoint){
    .x = v1.x - v2.x,
    .y = v1.y - v2.y,
  };
}

float v2f_mag(SDL_FPoint v){
  return sqrtf(v.x*v.x+v.y*v.y);
}

float v2f_mag2(SDL_FPoint v){
  return v.x*v.x + v.y*v.y;
}

SDL_FPoint v2f_normalize(SDL_FPoint v){
  float mag = v2f_mag(v);
  SDL_FPoint res = v;
  if (mag > 0.f){
    res.x /= mag;
    res.y /= mag;
  }
  return res;
}

SDL_FPoint v2f_mul(SDL_FPoint v1, SDL_FPoint v2){
  return (SDL_FPoint){
    .x = v1.x * v2.x,
    .y = v1.y * v2.y,
  };
}

SDL_FPoint v2f_mul_scalar(SDL_FPoint v1, float s){
  return (SDL_FPoint){
    .x = v1.x * s,
    .y = v1.y * s,
  };
}

// Sprite
int SDL_LoadSprite(SDL_Sprite* spr, const char* filename, size_t hframes, size_t vframes, SDL_Renderer* ren){
  int w,h,n;
  spr->data = stbi_load(filename, &w, &h, &n, 4);
  if (spr->data == NULL){
    fprintf(stderr, "ERROR: Could not load image '%s'\n", filename);
    return -1;
  }

  spr->origin = (SDL_FPoint){0.f, 0.f};
  spr->pos =  (SDL_FPoint){0.f, 0.f};
  spr->size = (SDL_FPoint){(float)w, (float)h};
  spr->scale = (SDL_FPoint){1.f, 1.f};
  spr->angle = 0.f;
  spr->vflip = false;
  spr->hflip = false;
  spr->vframe = 0;
  spr->hframe = 0;
  spr->vframes = vframes;
  spr->hframes = hframes;
  spr->ren_rect = (SDL_Rect){
    .x = 0,
    .y = 0,
    .w = (int)spr->size.x / hframes,
    .h = (int)spr->size.y / vframes,
  };
  spr->time_per_frame = 0.15f;
  spr->accumulated = 0.f;

  SDL_SpriteSetHFrame(spr, spr->hframe);
  SDL_SpriteSetVFrame(spr, spr->vframe);

  spr->tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);
  if (spr->tex == NULL){
    fprintf(stderr, "ERROR: Could not create texture: %s\n", SDL_GetError());
    return -1;
  }

  if (SDL_SetTextureBlendMode(spr->tex, SDL_BLENDMODE_BLEND) < 0){
    fprintf(stderr, "ERROR: SDL_SetTextureBlendMode() -> %s\n", SDL_GetError());
    return -1;
  }

  if (SDL_UpdateTexture(spr->tex, NULL, spr->data, n*w) < 0){
    fprintf(stderr, "ERROR: Could not update texture: %s\n", SDL_GetError());
    return -1;
  }

  return 0;
}

int SDL_RenderSprite(SDL_Renderer* ren, SDL_Sprite* spr){
  const SDL_FPoint scaled_origin = {
    .x = spr->origin.x * spr->scale.x,
    .y = spr->origin.y * spr->scale.y,
  };

  const SDL_FRect dstrect = {
    .x = spr->pos.x - scaled_origin.x,
    .y = spr->pos.y - scaled_origin.y,
    .w = spr->ren_rect.w * spr->scale.x,
    .h = spr->ren_rect.h * spr->scale.y,
  };

  SDL_RendererFlip flip = SDL_FLIP_NONE;
  if (spr->vflip && spr->hflip){
    flip = SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL;
  } else if (spr->vflip){
    flip = SDL_FLIP_VERTICAL;
  } else if (spr->hflip){
    flip = SDL_FLIP_HORIZONTAL;
  }

  if (SDL_RenderCopyExF(ren, spr->tex, &spr->ren_rect, &dstrect, spr->angle, &scaled_origin, flip) < 0){
    fprintf(stderr, "ERROR: SDL_RenderCopyF() -> %s\n", SDL_GetError());
    return -1;
  }

  return 0;
}

void SDL_DestroySprite(SDL_Sprite* spr){
  SDL_DestroyTexture(spr->tex);
  stbi_image_free(spr->data);
}

void SDL_SpriteCenterOrigin(SDL_Sprite* spr){
  spr->origin.x = (float)spr->ren_rect.w/2.f;
  spr->origin.y = (float)spr->ren_rect.h/2.f;
}

void SDL_SpriteSetScale(SDL_Sprite* spr, SDL_FPoint* scl){
  if (scl->x < SPR_MIN_SCALE) scl->x = SPR_MIN_SCALE;
  if (scl->y < SPR_MIN_SCALE) scl->y = SPR_MIN_SCALE;
  spr->scale.x = scl->x;
  spr->scale.y = scl->y;
}

void SDL_SpriteSetHFrame(SDL_Sprite* spr, size_t hframe){
  if (hframe > spr->hframes-1) hframe = spr->hframes;
  spr->hframe = hframe;
  spr->ren_rect.x = hframe*spr->ren_rect.w;
}

void SDL_SpriteSetVFrame(SDL_Sprite* spr, size_t vframe){
  if (vframe > spr->vframes-1) vframe = spr->vframes;
  spr->vframe = vframe;
  spr->ren_rect.y = vframe*spr->ren_rect.h;
}

void SDL_SpriteAnimate(SDL_Sprite* spr, float delta){
  spr->accumulated += delta;

  if (spr->accumulated >= spr->time_per_frame){
    spr->accumulated -= spr->time_per_frame;

    SDL_SpriteSetHFrame(spr, (spr->hframe+1)%(spr->hframes));
  }
}

// Math
float randomf(float start, float end){
  float r = (float)rand() / (float)RAND_MAX; // 0..1

  return start + r*(end-start);
}
