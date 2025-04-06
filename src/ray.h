#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <limits.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "utils.h"

typedef struct {
  float angle;
  float wall_hit_x;
  float wall_hit_y;
  float distance;
  bool was_wall_hit_vert;
  int wall_hit_texture;
} ray_t;

extern ray_t rays[NUM_RAYS];

bool is_ray_facing_up(float angle);
bool is_ray_facing_down(float angle);
bool is_ray_facing_left(float angle);
bool is_ray_facing_right(float angle);
void cast_all_rays(void);
void cast_ray(float ray_angle, int strip_id);
void render_map_rays(void);

#endif
