
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "defs.h"

typedef struct {
  bool is_running;
  int last_frame_time;
  float delta_time;
} game_state_t;

extern game_state_t game_state;

void update_game_state(void);

#endif
