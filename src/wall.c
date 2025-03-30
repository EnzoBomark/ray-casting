
#include "wall.h"

// Change the color intensity based on a factor value between 0 and 1
void change_color_intensity(color_t* color, float factor) {
    color_t a = (*color & 0xFF000000);
    color_t r = (*color & 0x00FF0000) * factor;
    color_t g = (*color & 0x0000FF00) * factor;
    color_t b = (*color & 0x000000FF) * factor;

    *color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

void render_wall_projection(void) {
    for (int x = 0; x < NUM_RAYS; x++) {
        float perp_distance = rays[x].distance * cos(rays[x].angle - player.rotation_angle);
        float projected_wall_height = (TILE_SIZE / perp_distance) * DIST_PROJ_PLANE;

        int wall_height = (int)projected_wall_height;

        int wall_top_pixel = (WINDOW_HEIGHT / 2) - (wall_height / 2);
        wall_top_pixel = wall_top_pixel < 0 ? 0 : wall_top_pixel;

        int wall_bottom_pixel = (WINDOW_HEIGHT / 2) + (wall_height / 2);
        wall_bottom_pixel = wall_bottom_pixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wall_bottom_pixel;

        // set the color of the ceiling
        for (int y = 0; y < wall_top_pixel; y++) {
            draw_pixel(x, y, 0xFF444444);
        }

        int texture_offset;
        if (rays[x].was_wall_hit_vert)
            texture_offset = (int)rays[x].wall_hit_y % TILE_SIZE;
        else
            texture_offset = (int)rays[x].wall_hit_x % TILE_SIZE;

        // get the correct texture id number from the map content
        int texture_num = rays[x].wall_hit_content - 1;

        int texture_width = wall_textures[texture_num].width;
        int texture_height = wall_textures[texture_num].height;

        // render the wall from wall_top_pixel to wall_bottom_pixel
        for (int y = wall_top_pixel; y < wall_bottom_pixel; y++) {
            int distance_from_top = y + (wall_height / 2) - (WINDOW_HEIGHT / 2);
            int texture_offset_y = distance_from_top * ((float)texture_height / wall_height);

            // set the color of the wall based on the color from the texture
            color_t texel_color = wall_textures[texture_num].texture_buffer[(texture_width * texture_offset_y) + texture_offset];

            // Make the pixel color darker if the ray hit was vertical
            if (rays[x].was_wall_hit_vert) {
                change_color_intensity(&texel_color, 0.7);
            }

            draw_pixel(x, y, texel_color);
        }

        // set the color of the floor
        for (int y = wall_bottom_pixel; y < WINDOW_HEIGHT; y++) {
            draw_pixel(x, y, 0xFF888888);
        }
    }
}
