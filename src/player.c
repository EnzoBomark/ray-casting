#include "player.h"

player_t player = {
  .x = WINDOW_WIDTH / 2,
  .y = WINDOW_HEIGHT / 2,
  .width = 10,
  .height = 10,
  .walk_velocity = 200,
  .rotation_angle = PI / 2,
  .viewbob_offset = 0,
  .walk_direction = 0,
  .strafe_direction = 0,
};

void player_movement(float delta_time) {
  float walk_move = player.walk_direction * player.walk_velocity * delta_time;
  float strafe_move = player.strafe_direction * player.walk_velocity * delta_time;

  // Normalize if moving diagonally
  float magnitude = sqrt(walk_move * walk_move + strafe_move * strafe_move);

  if (magnitude > player.walk_velocity * delta_time) {
    float scale = (player.walk_velocity * delta_time) / magnitude;
    walk_move *= scale;
    strafe_move *= scale;
  }


  float new_player_x = player.x + walk_move * cos(player.rotation_angle) - strafe_move * sin(player.rotation_angle);
  float new_player_y = player.y + walk_move * sin(player.rotation_angle) + strafe_move * cos(player.rotation_angle);

  if (!has_map_wall_at(new_player_x, new_player_y)) {
    player.x = new_player_x;
    player.y = new_player_y;
  }
}

float viewbob_timer = 0.0;                 // Time-based bobbing progress
const float viewbob_speed = 10.0;          // Speed of the bobbing (increase)
const float viewbob_amount = 3.0;          // Amount of up/down motion (increase)

void player_viewbob(float delta_time) {
  if (player.walk_direction != 0 || player.strafe_direction != 0) {
    viewbob_timer += delta_time * viewbob_speed;
  }

  float vertical_bob = sin(viewbob_timer) * viewbob_amount;

  if (player.walk_direction != 0 || player.strafe_direction != 0) {
    player.viewbob_offset = vertical_bob;
  } else {
    player.viewbob_offset = lerp(player.viewbob_offset, 0.0, delta_time * 10.0);
  }
}

void render_map_player(void) {
  draw_rect(
    (player.x * MINIMAP_SCALE_FACTOR) + MINIMAP_X_OFFSET,
    (player.y * MINIMAP_SCALE_FACTOR) + MINIMAP_Y_OFFSET,
    player.width * MINIMAP_SCALE_FACTOR,
    player.height * MINIMAP_SCALE_FACTOR,
    0xFFFFFFFF
  );
}