#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include "game-state.h"
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
  float weaponbob_offset;
  float weaponbob_scale;
  float move_dx;
  float move_dy;
  int walk_forward;
  int walk_backward;
  int strafe_left;
  int strafe_right;
} player_t;

extern player_t player;

void update_player(void);
void render_map_player(void);

#endif
