#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include "graphics.h"
#include "map.h"

typedef struct {
  float x;
  float y;
  float width;
  float height;
  float walk_velocity;
  float turn_velocity;
  float rotation_angle;
  int turn_direction; // -1 => left, +1 => right
  int walk_direction; // -1 => back, +1 => front
} player_t;

extern player_t player;

void move_player(float delta_time);
void render_player(void);

#endif
