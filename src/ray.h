#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <limits.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"

typedef struct {
    float angle;
    float wall_hit_x;
    float wall_hit_y;
    float distance;
    bool was_wall_hit_vert;
    int wall_hit_content;
} ray_t;

extern ray_t rays[NUM_RAYS];

void normalize_angle(float* angle);
float distance_between_points(float x1, float y1, float x2, float y2);
bool is_ray_facing_up(float angle);
bool is_ray_facing_down(float angle);
bool is_ray_facing_left(float angle);
bool is_ray_facing_right(float angle);
void cast_all_rays(void);
void cast_ray(float ray_angle, int strip_id);
void render_rays(void);

#endif
