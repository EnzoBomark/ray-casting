#include "sprite.h"

#define NUM_SPRITES 2

static sprite_t sprites[NUM_SPRITES] = {
  {.x = 640, .y = 630, .texture = 10 },
  {.x = 240, .y = 430, .texture = 11 }
};

void render_sprite_projection(void) {
  sprite_t visible_sprites[NUM_SPRITES];
  int num_visible_sprites = 0;

  for (int i = 0; i < NUM_SPRITES; i++) {
    float angle_between_sprite_and_player = player.rotation_angle - atan2(sprites[i].y - player.y, sprites[i].x - player.x);

    // Make sure angle is always between 0 and 180
    if (angle_between_sprite_and_player > PI) {
      angle_between_sprite_and_player -= TWO_PI;
    }

    if (angle_between_sprite_and_player < -PI) {
      angle_between_sprite_and_player += TWO_PI;
    }

    angle_between_sprite_and_player = fabs(angle_between_sprite_and_player);


    const float EPSILON = 0.2;
    if (angle_between_sprite_and_player < (FOV_ANGLE / 2) + EPSILON) {
      sprites[i].visible = true;
      sprites[i].angle = angle_between_sprite_and_player;
      sprites[i].distance = distance_between_points(sprites[i].x, sprites[i].y, player.x, player.y);
      visible_sprites[num_visible_sprites] = sprites[i];
      num_visible_sprites++;
    }
    else {
      sprites[i].visible = false;
    }
  }

  // bubble sort
  for (int i = 0; i < num_visible_sprites - 1; i++) {
    for (int j = 0 + 1; j < num_visible_sprites; j++) {
      if (visible_sprites[i].distance < visible_sprites[j].distance) {
        sprite_t temp = visible_sprites[i];
        visible_sprites[i] = visible_sprites[j];
        visible_sprites[j] = temp;
      }
    }
  }

  for (int i = 0; i < num_visible_sprites; i++) {
    sprite_t sprite = visible_sprites[i];

    float perp_distance = sprite.distance * cos(sprite.angle);
    float sprite_height = (TILE_SIZE / perp_distance) * DIST_PROJ_PLANE;
    float sprite_width = sprite_height;

    float sprite_top_y = (WINDOW_HEIGHT / 2) - (sprite_height / 2);
    sprite_top_y = (sprite_top_y < 0) ? 0 : sprite_top_y;

    float sprite_bottom_y = (WINDOW_HEIGHT / 2) + (sprite_height / 2);
    sprite_bottom_y = (sprite_bottom_y > WINDOW_HEIGHT) ? WINDOW_HEIGHT : sprite_bottom_y;

    float sprite_angle = atan2(sprite.y - player.y, sprite.x - player.x) - player.rotation_angle;
    float sprite_screen_pos_x = tan(sprite_angle) * DIST_PROJ_PLANE;
    float sprite_left_x = (WINDOW_WIDTH / 2) + sprite_screen_pos_x - (sprite_width / 2);
    float sprite_right_x = sprite_left_x + sprite_width;

    int texture_width = upng_get_width(textures[sprite.texture]);
    int texture_height = upng_get_height(textures[sprite.texture]);

    for (int x = sprite_left_x; x < sprite_right_x; x++) {
      float texel_width = (texture_width / sprite_width);
      int texture_offset_x = (x - sprite_left_x) * texel_width;

      for (int y = sprite_top_y; y < sprite_bottom_y; y++) {
        if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT) {
          int distance_from_top = y + (sprite_height / 2) - (WINDOW_HEIGHT / 2);
          int texture_offset_y = distance_from_top * ((float)texture_height / sprite_height);

          int buffer_position = (texture_width * texture_offset_y) + texture_offset_x;
          color_t* wall_texture_buffer = (color_t*)upng_get_buffer(textures[sprite.texture]);
          size_t buffer_size = upng_get_size(textures[sprite.texture]);

          if (buffer_position < 0) {
            buffer_position = 0;
          }

          if (buffer_position >= buffer_size / sizeof(color_t)) {
            buffer_position = (buffer_size / sizeof(color_t)) - 1;
          }

          color_t texel_color = wall_texture_buffer[buffer_position];

          if (sprite.distance > rays[x].distance) {
            continue;
          }

          if (texel_color == 0xFFFF00FF) {
            continue;
          }

          draw_pixel(x, y, texel_color);
        }
      }
    }
  }
}

void render_map_sprites(void) {
  for (int i = 0; i < NUM_SPRITES; i++) {
    draw_rect(
      sprites[i].x * MINIMAP_SCALE_FACTOR,
      sprites[i].y * MINIMAP_SCALE_FACTOR,
      3,
      3,
      (sprites[i].visible ? 0xFF00FF00 : 0xFF333333)
    );
  }
}