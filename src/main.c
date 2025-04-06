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

  move_player(delta_time);
  cast_all_rays();
};

float viewbob_timer = 0.0f;           // Time-based bobbing progress
float viewbob_speed = 70.0f;          // Speed of the bobbing (increase)
float viewbob_amount = 4.0f;         // Amount of up/down motion (increase)
float viewbob_step_intensity = 8.0f;  // Intensity of the "step thump" (increase)
float viewbob_step_frequency = 0.3f;  // How often the steps are "thumping" (0.3 = faster thump)
float step_timer = 0.0f;  // Time until the next step event

void render(void) {
  render_clear();
  clear_color_buffer(0xFFFF0000);

  float delta_time = (SDL_GetTicks() - game_state.last_frame_time) / 1000.0f;

  if (player.walk_direction != 0 || player.strafe_direction != 0) {
    viewbob_timer += delta_time * viewbob_speed;
    step_timer += delta_time;
  }

  float vertical_bob = sin(viewbob_timer) * viewbob_amount;

  if (step_timer >= viewbob_step_frequency) {
    vertical_bob += sin(viewbob_timer) * viewbob_step_intensity;
    step_timer = 0.0f;
  }

  static float smoothed_bob = 0.0f;

  if (player.walk_direction != 0 || player.strafe_direction != 0) {
    smoothed_bob = vertical_bob;
  } else {
    smoothed_bob = lerp(smoothed_bob, 0.0f, 0.1f);
  }

  float viewbob_offset_y = smoothed_bob;


  render_wall_projection(viewbob_offset_y);
  render_sprite_projection(viewbob_offset_y);

  char debug_text[256];
  snprintf(debug_text, sizeof(debug_text), "FPS: %d | Player X: %.2f | Player Y: %.2f | Angle: %.2f | Viewbob Y: %.2f",
    (int)(1.0f / delta_time), player.x, player.y, player.rotation_angle * (180.0f / M_PI), viewbob_offset_y);

  render_map_grid();
  render_map_rays();
  render_map_player();
  render_map_sprites();

  render_color_buffer();
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