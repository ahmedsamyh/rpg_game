#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "SDL_Texture_wrapper.h"

typedef struct {
  const char* key;
  SDL_Texture_wrapper value;
} SDL_Texture_wrapper_KV;

extern SDL_FRect* collboxes;
extern SDL_Texture_wrapper_KV* texture_map;
extern int screen_width;
extern int screen_height;
extern float width;
extern float scl;
extern float height;

#endif /* _GLOBALS_H_ */
