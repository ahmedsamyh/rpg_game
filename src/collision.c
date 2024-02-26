#include <collision.h>
#include <math.h>
#include <assert.h>

bool Check_rect_rect_collision(const SDL_FRect rect1, const SDL_FRect rect2){
  const float rect1_l = rect1.x;
  const float rect1_r = rect1.x+rect1.w;
  const float rect1_t = rect1.y;
  const float rect1_b = rect1.y+rect1.h;

  const float rect2_l = rect2.x;
  const float rect2_r = rect2.x+rect2.w;
  const float rect2_t = rect2.y;
  const float rect2_b = rect2.y+rect2.h;

  return (rect1_r >= rect2_l &&
	  rect1_l <= rect2_r &&
	  rect1_t <= rect2_b &&
	  rect1_b >= rect2_t);
}

bool Check_rect_rect_collision_ex(const SDL_FRect rect1, const SDL_FRect rect2, Coll_Axis *caxis){
  if (Check_rect_rect_collision(rect1, rect2)){
    const float rect1_l = rect1.x;
    const float rect1_r = rect1.x+rect1.w;
    const float rect1_t = rect1.y;
    const float rect1_b = rect1.y+rect1.h;

    const float rect2_l = rect2.x;
    const float rect2_r = rect2.x+rect2.w;
    const float rect2_t = rect2.y;
    const float rect2_b = rect2.y+rect2.h;

    SDL_FPoint cb2_bot = {0.f, rect2_b};
    SDL_FPoint cb1_top = {0.f, rect1_t};
    float d2_top = v2f_mag2(v2f_sub(cb1_top, cb2_bot));
    SDL_FPoint cb2_left = {rect2_l, 0.f};
    SDL_FPoint cb1_right = {rect1_r, 0.f};
    float d2_right = v2f_mag2(v2f_sub(cb1_right, cb2_left));
    SDL_FPoint cb2_right = {rect2_r, 0.f};
    SDL_FPoint cb1_left = {rect1_l, 0.f};
    float d2_left = v2f_mag2(v2f_sub(cb1_left, cb2_right));
    SDL_FPoint cb2_top = {0.f, rect2_t};
    SDL_FPoint cb1_bot = {0.f, rect1_b};
    float d2_bot = v2f_mag2(v2f_sub(cb1_bot, cb2_top));

    float min_d2 = fminf(d2_top, fminf(d2_left, fminf(d2_right, d2_bot)));

    if (min_d2 == d2_top || min_d2 == d2_bot){
      *caxis = COLL_AXIS_VERTICAL;
    } else if (min_d2 == d2_left || min_d2 == d2_right){
      *caxis = COLL_AXIS_HORIZONTAL;
    } else {
      assert(0 && "UNREACHABLE");
    }

    return true;
  }

  *caxis = COLL_AXIS_NONE;
  return false;
}

bool Resolve_rect_rect_collision(SDL_FRect* rect1, const SDL_FRect rect2){
  const float rect1_l = rect1->x;
  const float rect1_r = rect1->x+rect1->w;
  const float rect1_t = rect1->y;
  const float rect1_b = rect1->y+rect1->h;

  const float rect2_l = rect2.x;
  const float rect2_r = rect2.x+rect2.w;
  const float rect2_t = rect2.y;
  const float rect2_b = rect2.y+rect2.h;

  // resolve collision only if it ever happens
  if (Check_rect_rect_collision(*rect1, rect2)){
    SDL_FPoint cb2_bot = {0.f, rect2_b};
    SDL_FPoint cb1_top = {0.f, rect1_t};
    float d2_top = v2f_mag2(v2f_sub(cb1_top, cb2_bot));
    SDL_FPoint cb2_left = {rect2_l, 0.f};
    SDL_FPoint cb1_right = {rect1_r, 0.f};
    float d2_right = v2f_mag2(v2f_sub(cb1_right, cb2_left));
    SDL_FPoint cb2_right = {rect2_r, 0.f};
    SDL_FPoint cb1_left = {rect1_l, 0.f};
    float d2_left = v2f_mag2(v2f_sub(cb1_left, cb2_right));
    SDL_FPoint cb2_top = {0.f, rect2_t};
    SDL_FPoint cb1_bot = {0.f, rect1_b};
    float d2_bot = v2f_mag2(v2f_sub(cb1_bot, cb2_top));

    float min_d2 = fminf(d2_top, fminf(d2_left, fminf(d2_right, d2_bot)));

    if (min_d2 == d2_top){
      rect1->y = rect2_b;
    } else if (min_d2 == d2_left){
      rect1->x = rect2_r;
    } else if (min_d2 == d2_right){
      rect1->x = rect2_l - rect1->w;
    } else if (min_d2 == d2_bot){
      rect1->y = rect2_t - rect1->h;
    } else {
      assert(0 && "UNREACHABLE");
    }
    return true;
  }
  return false;
}

bool Check_rect_point_collision(const SDL_FRect rect, const SDL_FPoint point){
  const float rect_l = rect.x;
  const float rect_r = rect.x+rect.w;
  const float rect_t = rect.y;
  const float rect_b = rect.y+rect.h;

  return (point.x >= rect_l &&
	  point.x <= rect_r &&
	  point.y >= rect_t &&
	  point.y <= rect_b);
}
