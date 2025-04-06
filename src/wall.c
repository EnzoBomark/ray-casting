
#include "wall.h"

// Change the color intensity based on a factor value between 0 and 1
void change_color_intensity(color_t* color, float factor) {
  color_t a = (*color & 0xFF000000);
  color_t r = (*color & 0x00FF0000) * factor;
  color_t g = (*color & 0x0000FF00) * factor;
  color_t b = (*color & 0x000000FF) * factor;

  *color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

void render_wall_projection(float viewbob_offset_y) {

  for (int x = 0; x < NUM_RAYS; x++) {
    // calculate the perpendicular distance to avoid the fish-eye distortion
    float perp_distance = rays[x].distance * cos(rays[x].angle - player.rotation_angle);

    float wall_height = (TILE_SIZE / perp_distance) * DIST_PROJ_PLANE;

    int wall_top_y = (WINDOW_HEIGHT / 2) - (wall_height / 2) + viewbob_offset_y;
    wall_top_y = wall_top_y < 0 ? 0 : wall_top_y;

    int wall_bottom_y = (WINDOW_HEIGHT / 2) + (wall_height / 2) + viewbob_offset_y;
    wall_bottom_y = wall_bottom_y > WINDOW_HEIGHT ? WINDOW_HEIGHT : wall_bottom_y;

    // set the color of the ceiling
    for (int y = 0; y < wall_top_y; y++) {
      draw_pixel(x, y, 0xFF444444);
    }

    int texture_offset;

    if (rays[x].was_wall_hit_vert) {
      texture_offset = (int)rays[x].wall_hit_y % TILE_SIZE;
    } else {
      texture_offset = (int)rays[x].wall_hit_x % TILE_SIZE;
    }

    // get the correct texture id number from the map content
    int texture_num = rays[x].wall_hit_texture - 1;

    int texture_width = upng_get_width(textures[texture_num]);
    int texture_height = upng_get_height(textures[texture_num]);

    // render the wall from wall_top_y to wall_bottom_y
    for (int y = wall_top_y; y < wall_bottom_y; y++) {
      int distance_from_top = y + (wall_height / 2) - (WINDOW_HEIGHT / 2) - viewbob_offset_y;
      int texture_offset_y = (distance_from_top * ((float)texture_height / wall_height));

      // set the color of the wall based on the color from the texture
      color_t* wall_texture_buffer = (color_t*)upng_get_buffer(textures[texture_num]);
      color_t texel_color = wall_texture_buffer[(texture_width * texture_offset_y) + texture_offset];

      // Make the pixel color darker if the ray hit was vertical
      if (rays[x].was_wall_hit_vert) {
        change_color_intensity(&texel_color, 0.7);
      }

      draw_pixel(x, y, texel_color);
    }

    // set the color of the floor
    for (int y = wall_bottom_y; y < WINDOW_HEIGHT; y++) {
      draw_pixel(x, y, 0xFF888888);
    }
  }
}
