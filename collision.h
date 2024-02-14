#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "sdl_helper.h"

typedef enum {
  COLL_AXIS_NONE = 0,
  COLL_AXIS_VERTICAL,
  COLL_AXIS_HORIZONTAL,
  COLL_AXIS_COUNT,
} Coll_Axis;

bool Check_rect_rect_collision(const SDL_FRect rect1, const SDL_FRect rect2);
bool Check_rect_rect_collision_ex(const SDL_FRect rect1, const SDL_FRect rect2, Coll_Axis *caxis);
bool Resolve_rect_rect_collision(SDL_FRect* rect1, const SDL_FRect rect2);
bool Check_rect_point_collision(const SDL_FRect rect, const SDL_FPoint point);

#endif /* _COLLISION_H_ */
