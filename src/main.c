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

void debug(void) {
  char debug_text[256];
  snprintf(debug_text, sizeof(debug_text), "Ammo: %d | Max Ammo: %d", weapon.ammo, weapon.max_ammo);
  render_debug_menu(debug_text);
}

void setup(void) {
  init_window();
  load_font();
  load_textures();
  load_weapons();
};

void release(void) {
  destroy_window();
}


void update(void) {
  update_game_state();
  update_player();
  update_weapon();
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

  debug();
  render_present();
};


int main() {
  setup();

  while (game_state.is_running) {
    process_input();
    update();
    render();
  }

  release();

  return 0;
}