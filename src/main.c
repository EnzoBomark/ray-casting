#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "defs.h"
#include "graphics.h"
#include "map.h"
#include "player.h"
#include "ray.h"
#include "wall.h"
#include "sprite.h"
#include "texture.h"
#include "font.h"

typedef struct {
  bool is_running;
  int last_frame_time;
} GameState;

static GameState game_state = {
  .is_running = true,
  .last_frame_time = 0,
};

void on_key_down(SDL_Keycode code) {
  switch (code) {
  case SDLK_ESCAPE:
    game_state.is_running = false;
    break;
  case SDLK_w:
    player.walk_direction = +1;
    break;
  case SDLK_s:
    player.walk_direction = -1;
    break;
  case SDLK_d:
    player.strafe_direction = +1;;
    break;
  case SDLK_a:
    player.strafe_direction = -1;
    break;
  default:
    break;
  }
}

void on_key_up(SDL_Keycode code) {
  switch (code) {
  case SDLK_w:
    player.walk_direction = 0;
    break;
  case SDLK_s:
    player.walk_direction = 0;
    break;
  case SDLK_d:
    player.strafe_direction = 0;
    break;
  case SDLK_a:
    player.strafe_direction = 0;
    break;
  default:
    break;
  }
}

void process_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      game_state.is_running = false;
      break;
    case SDL_KEYDOWN:
      on_key_down(event.key.keysym.sym);
      break;
    case SDL_KEYUP:
      on_key_up(event.key.keysym.sym);
      break;
    case SDL_MOUSEMOTION:
      player.rotation_angle += (event.motion.xrel * 0.001f);
      normalize_angle(&player.rotation_angle);
      break;
    default:
      break;
    }
  }
}

void setup(void) {
  load_font();
  load_textures();
};

void update(void) {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - game_state.last_frame_time);

  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  float delta_time = (SDL_GetTicks() - game_state.last_frame_time) / 1000.0f;
  game_state.last_frame_time = SDL_GetTicks();

  player_movement(delta_time);
  player_viewbob(delta_time);
  cast_all_rays();
};

void render(void) {
  clear_color_buffer(0xFFFF0000);
  render_clear();

  render_wall_projection();
  render_sprite_projection();

  render_map_grid();
  render_map_rays();
  render_map_player();
  render_map_sprites();

  render_color_buffer();

  char debug_text[256];
  snprintf(debug_text, sizeof(debug_text), "Player X: %.2f | Player Y: %.2f | Angle: %.2f | Viewbob offset: %.2f", player.x, player.y, player.rotation_angle * (180.0f / M_PI), player.viewbob_offset);
  render_debug_menu(debug_text);

  render_present();
};

void release_resourses(void) {
  destroy_window();
}

void game_loop(void) {
  setup();

  while (game_state.is_running) {
    process_input();
    update();
    render();
  }
}

int main() {
  initialize_window();
  game_loop();
  release_resourses();
  return 0;
}