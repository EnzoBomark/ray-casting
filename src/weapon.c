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
  weapon = handgun;
}

void process_weapon_input(void) {
  if (is_mouse_button_pressed(1) || is_mouse_button_down(1)) {
    fire_weapon();
  }
}

float last_shot_time = 0.0;
float fire_animation_timer = 0.0;
int fire_animation_frame = 0;
bool is_playing_fire_animation = false;

void fire_weapon(void) {
  if (weapon.ammo <= 0) {
    weapon.ammo = weapon.max_ammo;
  }

  float time_since_last_shot = (SDL_GetTicks() - last_shot_time) / 1000.0f;

  if (time_since_last_shot < weapon.fire_rate) {
    return;
  }

  weapon.ammo--;
  is_playing_fire_animation = true;
  fire_animation_timer = 0.0f;
  last_shot_time = SDL_GetTicks();
}

void animate_weapon(void) {
  if (is_playing_fire_animation) {
    fire_animation_timer += game_state.delta_time;

    float frame_duration = weapon.fire_rate / 4.0f;

    if (fire_animation_frame == 0 || fire_animation_timer >= frame_duration) {
      fire_animation_timer = 0.0f;
      fire_animation_frame++;

      if (fire_animation_frame >= 4) {
        fire_animation_frame = 0;
        is_playing_fire_animation = false;
      }
    }

    weapon.current_texture = fire_animation_frame;
  } else {
    weapon.current_texture = 0;
  }
}



void update_weapon(void) {
  process_weapon_input();
  animate_weapon();
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