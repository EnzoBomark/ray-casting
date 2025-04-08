#include "player.h"

player_t player = {
  .x = WINDOW_WIDTH / 2,
  .y = WINDOW_HEIGHT / 2,
  .width = 10.0f,
  .height = 10.0f,
  .walk_velocity = 200.0f,
  .rotation_angle = PI / 2,
  .viewbob_offset = 0.0f,
  .weaponbob_offset = 0.0f,
  .weaponbob_scale = 0.0f,
  .walk_forward = 0,
  .walk_backward = 0,
  .strafe_left = 0,
  .strafe_right = 0,
};

void process_player_input(void) {
  player.rotation_angle += get_mouse_motion_rel_x();
  normalize_angle(&player.rotation_angle);

  if (is_key_pressed(SDLK_w) || is_key_down(SDLK_w)) {
    player.walk_forward = 1;
  } else {
    player.walk_forward = 0;
  }

  if (is_key_pressed(SDLK_s) || is_key_down(SDLK_s)) {
    player.walk_backward = 1;
  } else {
    player.walk_backward = 0;
  }

  if (is_key_pressed(SDLK_d) || is_key_down(SDLK_d)) {
    player.strafe_right = 1;
  } else {
    player.strafe_right = 0;
  }

  if (is_key_pressed(SDLK_a) || is_key_down(SDLK_a)) {
    player.strafe_left = 1;
  } else {
    player.strafe_left = 0;
  }
}

const float SMOOTHING = 15.0f;

void player_movement(void) {
  int walk_direction = player.walk_forward - player.walk_backward;
  int strafe_direction = player.strafe_right - player.strafe_left;

  float target_walk_move = walk_direction * player.walk_velocity * game_state.delta_time;
  float target_strafe_move = strafe_direction * player.walk_velocity * game_state.delta_time;

  // Normalize if moving diagonally
  float magnitude = sqrt(target_walk_move * target_walk_move + target_strafe_move * target_strafe_move);

  if (magnitude > player.walk_velocity * game_state.delta_time) {
    float scale = (player.walk_velocity * game_state.delta_time) / magnitude;
    target_walk_move *= scale;
    target_strafe_move *= scale;
  }

  // Rotate the movement based on player facing
  float target_move_dx = target_walk_move * cos(player.rotation_angle) - target_strafe_move * sin(player.rotation_angle);
  float target_move_dy = target_walk_move * sin(player.rotation_angle) + target_strafe_move * cos(player.rotation_angle);

  // Smoothly interpolate current movement towards target movement
  player.move_dx = lerp(player.move_dx, target_move_dx, game_state.delta_time * SMOOTHING);
  player.move_dy = lerp(player.move_dy, target_move_dy, game_state.delta_time * SMOOTHING);

  float new_player_x = player.x + player.move_dx;
  float new_player_y = player.y + player.move_dy;

  if (!has_map_wall_at(new_player_x, new_player_y)) {
    player.x = new_player_x;
    player.y = new_player_y;
  } else {
    if (!has_map_wall_at(player.x + player.move_dx, player.y)) {
      player.x += player.move_dx;
    }

    if (!has_map_wall_at(player.x, player.y + player.move_dy)) {
      player.y += player.move_dy;
    }
  }
}

float viewbob_timer = 0.0f;
const float viewbob_speed = 20.0f;
const float viewbob_amount = 2.0f;

void player_viewbob(void) {
  int walk_direction = player.walk_forward - player.walk_backward;
  int strafe_direction = player.strafe_left - player.strafe_right;

  if (walk_direction != 0 || strafe_direction != 0) {
    viewbob_timer += game_state.delta_time * viewbob_speed;
  }

  float vertical_bob = sin(viewbob_timer) * viewbob_amount;

  if (walk_direction != 0 || strafe_direction != 0) {
    player.viewbob_offset = vertical_bob;
  } else {
    player.viewbob_offset = lerp(player.viewbob_offset, 0.0f, game_state.delta_time * 10.0f);
  }
}

float weaponbob_offset_timer = 0.0f;
const float weaponbob_offset_speed = 10.0f;
const float weaponbob_offset_amount = 5.0f;

float weaponbob_scale_timer = 0.0f;
const float weaponbob_scale_speed = 20.0f;
const float weaponbob_scale_amount = 0.1f;

void player_weaponbob(void) {
  int walk_direction = player.walk_forward - player.walk_backward;
  int strafe_direction = player.strafe_left - player.strafe_right;

  if (walk_direction != 0 || strafe_direction != 0) {
    weaponbob_offset_timer += game_state.delta_time * weaponbob_offset_speed;
    weaponbob_scale_timer += game_state.delta_time * weaponbob_scale_speed;
  }

  float horizontal = sin(weaponbob_offset_timer) * weaponbob_offset_amount;
  float scale = (sin(weaponbob_scale_timer) * weaponbob_scale_amount);

  if (walk_direction != 0 || strafe_direction != 0) {
    player.weaponbob_offset = horizontal;
    player.weaponbob_scale = scale;
  } else {
    player.weaponbob_offset = lerp(player.weaponbob_offset, 0.0f, game_state.delta_time * 10.0f);
    player.weaponbob_scale = lerp(player.weaponbob_scale, 0.0f, game_state.delta_time * 10.0f);
  }
}

void update_player(void) {
  process_player_input();
  player_movement();
  player_viewbob();
  player_weaponbob();
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