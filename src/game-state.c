#include "game-state.h"

game_state_t game_state = {
 .is_running = true,
 .last_frame_time = 0,
 .delta_time = 0.0f
};

void update_game_state(void) {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - game_state.last_frame_time);

  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  game_state.delta_time = (SDL_GetTicks() - game_state.last_frame_time) / 1000.0f;
  game_state.last_frame_time = SDL_GetTicks();
}

