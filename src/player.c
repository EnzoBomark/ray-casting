#include "player.h"

player_t player = {
  .x = WINDOW_WIDTH / 2,
  .y = WINDOW_HEIGHT / 2,
  .width = 10,
  .height = 10,
  .turn_direction = 0,
  .walk_direction = 0,
  .rotation_angle = PI / 2,
  .walk_velocity = 170,
  .turn_velocity = 120 * (PI / 180),
};

void move_player(float delta_time) {
  player.rotation_angle += player.turn_direction * player.turn_velocity * delta_time;
  normalize_angle(&player.rotation_angle);

  float move = player.walk_direction * player.walk_velocity * delta_time;
  float new_player_x = player.x + move * cos(player.rotation_angle);
  float new_player_y = player.y + move * sin(player.rotation_angle);

  if (!has_map_wall_at(new_player_x, new_player_y)) {
    player.x = new_player_x;
    player.y = new_player_y;
  }
}

void render_map_player(void) {
  draw_rect(
    player.x * MINIMAP_SCALE_FACTOR,
    player.y * MINIMAP_SCALE_FACTOR,
    player.width * MINIMAP_SCALE_FACTOR,
    player.height * MINIMAP_SCALE_FACTOR,
    0xFFFFFFFF
  );
}