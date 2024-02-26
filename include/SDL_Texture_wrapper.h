#ifndef _SDL_TEXTURE_WRAPPER_H_
#define _SDL_TEXTURE_WRAPPER_H_

#define TEXTURE_MAX_TRY_LOAD 1

typedef struct {
  int w,h,n;
  unsigned char* data;
  size_t size;
  SDL_Texture* tex;
  size_t num_try_loaded;
} SDL_Texture_wrapper;


#endif /* _SDL_TEXTURE_WRAPPER_H_ */
