#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include "graphics.h"
#include "map.h"
#include "utils.h"

typedef struct {
  float x;
  float y;
  float width;
  float height;
  float walk_velocity;
  float rotation_angle;
  float viewbob_offset;
  int walk_direction; // -1 => back, +1 => front
  int strafe_direction; // -1 => left, +1 => right
} player_t;

extern player_t player;

void player_movement(float delta_time);
void player_viewbob(float delta_time);
void render_map_player(void);

#endif
