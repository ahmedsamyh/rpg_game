#ifndef _SDL_TEXTURE_WRAPPER_H_
#define _SDL_TEXTURE_WRAPPER_H_

typedef struct {
  int w,h,n;
  unsigned char* data;
  size_t size;
  SDL_Texture* tex;
} SDL_Texture_wrapper;


#endif /* _SDL_TEXTURE_WRAPPER_H_ */
