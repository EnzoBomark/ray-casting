
#include "ray.h"

ray_t rays[NUM_RAYS];

bool is_ray_facing_down(float angle) {
  return angle > 0 && angle < PI;
}

bool is_ray_facing_up(float angle) {
  return !is_ray_facing_down(angle);
}

bool is_ray_facing_right(float angle) {
  return angle < 0.5 * PI || angle > 1.5 * PI;
}

bool is_ray_facing_left(float angle) {
  return !is_ray_facing_right(angle);
}

void cast_ray(float ray_angle, int ray_index) {
  normalize_angle(&ray_angle);

  float x_intercept, y_intercept;
  float x_step, y_step;

  int found_horz_wall_hit = false;
  int horz_wall_texture = 0;
  float horz_wall_hit_x = 0;
  float horz_wall_hit_y = 0;

  y_intercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
  y_intercept += is_ray_facing_down(ray_angle) ? TILE_SIZE : 0;

  x_intercept = player.x + (y_intercept - player.y) / tan(ray_angle);

  y_step = TILE_SIZE;
  y_step *= is_ray_facing_up(ray_angle) ? -1 : 1;

  x_step = TILE_SIZE / tan(ray_angle);
  x_step *= (is_ray_facing_left(ray_angle) && x_step > 0) ? -1 : 1;
  x_step *= (is_ray_facing_right(ray_angle) && x_step < 0) ? -1 : 1;

  float next_horz_touch_x = x_intercept;
  float next_horz_touch_y = y_intercept;

  while (is_inside_map(next_horz_touch_x, next_horz_touch_y)) {
    float x_to_check = next_horz_touch_x;
    float y_to_check = next_horz_touch_y - (is_ray_facing_up(ray_angle) ? 1 : 0);

    if (has_map_wall_at(x_to_check, y_to_check)) {
      horz_wall_hit_x = next_horz_touch_x;
      horz_wall_hit_y = next_horz_touch_y;
      horz_wall_texture = get_map_wall_at(x_to_check, y_to_check);
      found_horz_wall_hit = true;
      break;
    } else {
      next_horz_touch_x += x_step;
      next_horz_touch_y += y_step;
    }
  }

  int found_vert_wall_hit = false;
  int vert_wall_texture = 0;
  float vert_wall_hit_x = 0;
  float vert_wall_hit_y = 0;

  x_intercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
  x_intercept += is_ray_facing_right(ray_angle) ? TILE_SIZE : 0;

  y_intercept = player.y + (x_intercept - player.x) * tan(ray_angle);

  x_step = TILE_SIZE;
  x_step *= is_ray_facing_left(ray_angle) ? -1 : 1;

  y_step = TILE_SIZE * tan(ray_angle);
  y_step *= (is_ray_facing_up(ray_angle) && y_step > 0) ? -1 : 1;
  y_step *= (is_ray_facing_down(ray_angle) && y_step < 0) ? -1 : 1;

  float next_vert_touch_x = x_intercept;
  float next_vert_touch_y = y_intercept;

  while (is_inside_map(next_vert_touch_x, next_vert_touch_y)) {
    float x_to_check = next_vert_touch_x - (is_ray_facing_left(ray_angle) ? 1 : 0);
    float y_to_check = next_vert_touch_y;

    if (has_map_wall_at(x_to_check, y_to_check)) {
      vert_wall_hit_x = next_vert_touch_x;
      vert_wall_hit_y = next_vert_touch_y;
      vert_wall_texture = get_map_wall_at(x_to_check, y_to_check);
      found_vert_wall_hit = true;
      break;
    } else {
      next_vert_touch_x += x_step;
      next_vert_touch_y += y_step;
    }
  }

  float horz_hit_distance = found_horz_wall_hit
    ? distance_between_points(player.x, player.y, horz_wall_hit_x, horz_wall_hit_y)
    : FLT_MAX;

  float vert_hit_distance = found_vert_wall_hit
    ? distance_between_points(player.x, player.y, vert_wall_hit_x, vert_wall_hit_y)
    : FLT_MAX;

  if (horz_hit_distance < vert_hit_distance) {
    rays[ray_index].wall_hit_texture = horz_wall_texture;
    rays[ray_index].wall_hit_x = horz_wall_hit_x;
    rays[ray_index].wall_hit_y = horz_wall_hit_y;
    rays[ray_index].distance = horz_hit_distance;
    rays[ray_index].was_wall_hit_vert = false;
    rays[ray_index].angle = ray_angle;
  } else {
    rays[ray_index].wall_hit_texture = vert_wall_texture;
    rays[ray_index].wall_hit_x = vert_wall_hit_x;
    rays[ray_index].wall_hit_y = vert_wall_hit_y;
    rays[ray_index].distance = vert_hit_distance;
    rays[ray_index].was_wall_hit_vert = true;
    rays[ray_index].angle = ray_angle;
  }

}

void cast_all_rays(void) {
  for (int col = 0; col < NUM_RAYS; col++) {
    float ray_angle = player.rotation_angle + atan((col - (NUM_RAYS / 2)) / DIST_PROJ_PLANE);
    cast_ray(ray_angle, col);
  }
}

void render_map_rays(void) {
  for (int i = 0; i < NUM_RAYS; i += 50) {
    draw_line(
      (player.x * MINIMAP_SCALE_FACTOR) + MINIMAP_X_OFFSET,
      (player.y * MINIMAP_SCALE_FACTOR) + MINIMAP_Y_OFFSET,
      (rays[i].wall_hit_x * MINIMAP_SCALE_FACTOR) + MINIMAP_X_OFFSET,
      (rays[i].wall_hit_y * MINIMAP_SCALE_FACTOR) + MINIMAP_Y_OFFSET,
      0xFF0000FF
    );
  }
}
