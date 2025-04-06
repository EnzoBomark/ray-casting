#include "weapon.h"

weapon_t weapon;

weapon_t handgun;

void init_handgun(void) {
  handgun.id = 0;
  handgun.ammo = 15;
  handgun.max_ammo = 15;
  handgun.damage = 10;
  handgun.fire_rate = 0.5f;
  handgun.reload_time = 1.5f;
  handgun.current_texture = 0;

  handgun.textures[0] = textures[13];
  handgun.textures[1] = textures[14];
  handgun.textures[2] = textures[15];
  handgun.textures[3] = textures[16];
  handgun.textures[4] = textures[17];
}

void load_weapons(void) {
  init_handgun();

  // Set the initial weapon to handgun
  weapon = handgun;
}

void render_weapon(void) {
  float scale_factor = 12.0f + player.weaponbob_scale;

  int texture_width = upng_get_width(weapon.textures[weapon.current_texture]);
  int texture_height = upng_get_height(weapon.textures[weapon.current_texture]);

  int scaled_width = (int)(texture_width * scale_factor);
  int scaled_height = (int)(texture_height * scale_factor);

  int weapon_left_x = (WINDOW_WIDTH / 2) - (scaled_width / 2);
  int weapon_top_y = WINDOW_HEIGHT - scaled_height;

  color_t* handgun_texture_buffer = (color_t*)upng_get_buffer(weapon.textures[weapon.current_texture]);
  size_t buffer_size = upng_get_size(weapon.textures[weapon.current_texture]);

  for (int x = 0; x < scaled_width; x++) {
    for (int y = 0; y < scaled_height; y++) {
      int x_pos = x + weapon_left_x + player.weaponbob_offset;
      int y_pos = y + weapon_top_y;

      if (x_pos > 0 && x_pos < WINDOW_WIDTH && y_pos > 0 && y_pos < WINDOW_HEIGHT) {
        int original_x = (int)(x / scale_factor);
        int original_y = (int)(y / scale_factor);

        int buffer_position = (texture_width * original_y) + original_x;

        if (buffer_position < 0) {
          buffer_position = 0;
        }

        if (buffer_position >= buffer_size / sizeof(color_t)) {
          buffer_position = (buffer_size / sizeof(color_t)) - 1;
        }

        color_t texel_color = handgun_texture_buffer[buffer_position];

        if (texel_color == 0xFFFF00FF) {
          continue;
        }

        draw_pixel(x_pos, y_pos, texel_color);
      }
    }
  }
}


void fire_weapon(void) {
  if (weapon.ammo > 0) {
    weapon.ammo--;
  }
}

void reload_weapon(void) {
  if (weapon.ammo < weapon.max_ammo) {
    weapon.ammo = weapon.max_ammo;
  }
}