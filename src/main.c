#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "defs.h"
#include "font.h"
#include "game-state.h"
#include "graphics.h"
#include "map.h"
#include "player.h"
#include "ray.h"
#include "sprite.h"
#include "texture.h"
#include "wall.h"
#include "weapon.h"


void on_key_down(SDL_Keycode code) {
  switch (code) {
  case SDLK_ESCAPE:
    game_state.is_running = false;
    break;
  case SDLK_w:
    player.walk_forward = 1;
    break;
  case SDLK_s:
    player.walk_backward = 1;
    break;
  case SDLK_d:
    player.strafe_right = 1;;
    break;
  case SDLK_a:
    player.strafe_left = 1;
    break;
  default:
    break;
  }
}

void on_key_up(SDL_Keycode code) {
  switch (code) {
  case SDLK_w:
    player.walk_forward = 0;
    break;
  case SDLK_s:
    player.walk_backward = 0;
    break;
  case SDLK_d:
    player.strafe_right = 0;;
    break;
  case SDLK_a:
    player.strafe_left = 0;
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
    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        fire_weapon();
      }
      break;
    default:
      break;
    }
  }
}

void setup(void) {
  load_font();
  load_textures();
  load_weapons();
};

void update(void) {
  update_game_state();

  update_weapon();

  player_movement();
  player_viewbob();
  player_weaponbob();

  cast_all_rays();
};

void render(void) {
  clear_color_buffer(0xFFFF0000);
  render_clear();

  render_wall_projection();
  render_sprite_projection();
  render_weapon();

  // render_map_grid();
  // render_map_rays();
  // render_map_player();
  // render_map_sprites();

  render_color_buffer();

  char debug_text[256];
  int fps = 1000 / (SDL_GetTicks() - game_state.last_frame_time);
  snprintf(debug_text, sizeof(debug_text), "FPS: %d | Ammo: %d | Max Ammo: %d", fps, weapon.ammo, weapon.max_ammo);
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