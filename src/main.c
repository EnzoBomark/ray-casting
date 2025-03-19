#include <stdio.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include "./constants.h"

typedef struct {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* color_buffer_texture;
  Uint32* color_buffer;
  int last_frame_time;
  int quit;
} System;

static System sys = {
  .renderer = NULL,
  .window = NULL,
  .color_buffer_texture = NULL,
  .color_buffer = NULL,
  .last_frame_time = 0,
  .quit = FALSE,
};

void initialize_window(void) {
  ASSERT(!SDL_Init(SDL_INIT_EVERYTHING), "Error initializing SDL. %s", SDL_GetError());

  sys.window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_BORDERLESS
  );

  ASSERT(sys.window, "Error creating SDL Window. %s", SDL_GetError());

  sys.renderer = SDL_CreateRenderer(sys.window, -1, 0);

  ASSERT(sys.renderer, "Error creating SDL Renderer. %s", SDL_GetError());
}

void destroy_window() {
  free(sys.color_buffer);
  SDL_DestroyTexture(sys.color_buffer_texture);
  SDL_DestroyRenderer(sys.renderer);
  SDL_DestroyWindow(sys.window);
  SDL_Quit();
}

void render_color_buffer() {
  SDL_UpdateTexture(
    sys.color_buffer_texture,
    NULL,
    sys.color_buffer,
    (int)((Uint32)WINDOW_WIDTH * sizeof(Uint32))
  );

  SDL_RenderCopy(
    sys.renderer,
    sys.color_buffer_texture,
    NULL,
    NULL
  );
}

void clear_color_buffer(Uint32 color) {
  for (int x = 0; x < WINDOW_WIDTH; x++) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
      sys.color_buffer[(WINDOW_WIDTH * y) + x] = color;
    }
  }
}

const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int has_map_wall_at(float x, float y) {
  if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
    return TRUE;
  }

  int x_gird_index = floor(x / TILE_SIZE);
  int y_gird_index = floor(y / TILE_SIZE);
  return map[y_gird_index][x_gird_index] != 0;
}

int get_map_wall_at(float x, float y) {
  if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
    return 0;
  }

  int x_gird_index = floor(x / TILE_SIZE);
  int y_gird_index = floor(y / TILE_SIZE);
  return map[y_gird_index][x_gird_index];
}

void render_map() {
  for (int y = 0; y < MAP_NUM_ROWS; y++) {
    for (int x = 0; x < MAP_NUM_COLS; x++) {
      int tile_x = x * TILE_SIZE;
      int tile_y = y * TILE_SIZE;
      int tile_color = map[y][x] != 0 ? 255 : 0;

      SDL_SetRenderDrawColor(sys.renderer, tile_color, tile_color, tile_color, 255);

      SDL_Rect map_tile_rect = {
        tile_x * MINIMAP_SCALE_FACTOR,
        tile_y * MINIMAP_SCALE_FACTOR,
        TILE_SIZE * MINIMAP_SCALE_FACTOR,
        TILE_SIZE * MINIMAP_SCALE_FACTOR
      };

      SDL_RenderFillRect(sys.renderer, &map_tile_rect);
    }
  }
}

struct Player {
  float x;
  float y;
  float width;
  float height;
  float walk_velocity;
  float turn_velocity;
  float rotation_angle;
  int turn_direction; // -1 => left, +1 => right
  int walk_direction; // -1 => back, +1 => front
} player;

void move_player(struct Player* player, float delta_time) {
  player->rotation_angle += player->turn_direction * player->turn_velocity * delta_time;
  float move = player->walk_direction * player->walk_velocity * delta_time;
  float new_player_x = player->x + move * cos(player->rotation_angle);
  float new_player_y = player->y + move * sin(player->rotation_angle);

  if (!has_map_wall_at(new_player_x, new_player_y)) {
    player->x = new_player_x;
    player->y = new_player_y;
  }
}

void render_player() {
  SDL_SetRenderDrawColor(sys.renderer, 255, 0, 0, 255);

  SDL_Rect player_rect = {
    player.x * MINIMAP_SCALE_FACTOR,
    player.y * MINIMAP_SCALE_FACTOR,
    player.width * MINIMAP_SCALE_FACTOR,
    player.height * MINIMAP_SCALE_FACTOR,
  };

  SDL_RenderFillRect(sys.renderer, &player_rect);
}

struct Ray {
  float angle;
  float distance;
  float wall_hit_x;
  float wall_hit_y;
  int wall_hit_content;
  int was_hit_vert;
  int is_ray_facing_up;
  int is_ray_facing_down;
  int is_ray_facing_right;
  int is_ray_facing_left;
} rays[NUM_RAYS];

float normalize_angle(float angle) {
  angle = remainder(angle, TWO_PI);

  if (angle < 0) {
    angle = TWO_PI + angle;
  }

  return angle;
}

float distance_between_points(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void cast_ray(float ray_angle, int ray_index) {
  ray_angle = normalize_angle(ray_angle);

  int is_ray_facing_down = ray_angle > 0 && ray_angle < PI;
  int is_ray_facing_up = !is_ray_facing_down;

  int is_ray_facing_right = ray_angle < 0.5 * PI || ray_angle > 1.5 * PI;
  int is_ray_facing_left = !is_ray_facing_right;

  float x_intercept, y_intercept;
  float x_step, y_step;

  int found_horz_wall_hit = FALSE;
  int horz_wall_content = 0;
  float horz_wall_hit_x = 0;
  float horz_wall_hit_y = 0;

  y_intercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
  y_intercept += is_ray_facing_down ? TILE_SIZE : 0;

  x_intercept = player.x + (y_intercept - player.y) / tan(ray_angle);

  y_step = TILE_SIZE;
  y_step *= is_ray_facing_up ? -1 : 1;

  x_step = TILE_SIZE / tan(ray_angle);
  x_step *= (is_ray_facing_left && x_step > 0) ? -1 : 1;
  x_step *= (is_ray_facing_right && x_step < 0) ? -1 : 1;

  float next_horz_touch_x = x_intercept;
  float next_horz_touch_y = y_intercept;

  while (
    next_horz_touch_x >= 0 &&
    next_horz_touch_x <= WINDOW_WIDTH &&
    next_horz_touch_y >= 0 &&
    next_horz_touch_y <= WINDOW_HEIGHT
    ) {
    float x_to_check = next_horz_touch_x;
    float y_to_check = next_horz_touch_y - (is_ray_facing_up ? 1 : 0);
    int wall_content = get_map_wall_at(x_to_check, y_to_check);

    if (wall_content != 0) {
      found_horz_wall_hit = TRUE;
      horz_wall_content = wall_content;
      horz_wall_hit_x = next_horz_touch_x;
      horz_wall_hit_y = next_horz_touch_y;
      break;
    }
    else {
      next_horz_touch_x += x_step;
      next_horz_touch_y += y_step;
    }
  }

  int found_vert_wall_hit = FALSE;
  int vert_wall_content = 0;
  float vert_wall_hit_x = 0;
  float vert_wall_hit_y = 0;

  x_intercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
  x_intercept += is_ray_facing_right ? TILE_SIZE : 0;

  y_intercept = player.y + (x_intercept - player.x) * tan(ray_angle);

  x_step = TILE_SIZE;
  x_step *= is_ray_facing_left ? -1 : 1;

  y_step = TILE_SIZE * tan(ray_angle);
  y_step *= (is_ray_facing_up && y_step > 0) ? -1 : 1;
  y_step *= (is_ray_facing_down && y_step < 0) ? -1 : 1;

  float next_vert_touch_x = x_intercept;
  float next_vert_touch_y = y_intercept;

  while (
    next_vert_touch_x >= 0 &&
    next_vert_touch_x <= WINDOW_WIDTH &&
    next_vert_touch_y >= 0 &&
    next_vert_touch_y <= WINDOW_HEIGHT
    ) {
    float x_to_check = next_vert_touch_x - (is_ray_facing_left ? 1 : 0);
    float y_to_check = next_vert_touch_y;
    int wall_content = get_map_wall_at(x_to_check, y_to_check);

    if (wall_content != 0) {
      found_vert_wall_hit = TRUE;
      vert_wall_content = wall_content;
      vert_wall_hit_x = next_vert_touch_x;
      vert_wall_hit_y = next_vert_touch_y;
      break;
    }
    else {
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
    rays[ray_index].wall_hit_content = horz_wall_content;
    rays[ray_index].wall_hit_x = horz_wall_hit_x;
    rays[ray_index].wall_hit_y = horz_wall_hit_y;
    rays[ray_index].distance = horz_hit_distance;
    rays[ray_index].was_hit_vert = TRUE;
  }
  else {
    rays[ray_index].wall_hit_content = vert_wall_content;
    rays[ray_index].wall_hit_x = vert_wall_hit_x;
    rays[ray_index].wall_hit_y = vert_wall_hit_y;
    rays[ray_index].distance = vert_hit_distance;
    rays[ray_index].was_hit_vert = FALSE;
  }

  rays[ray_index].angle = ray_angle;
  rays[ray_index].is_ray_facing_down = is_ray_facing_down;
  rays[ray_index].is_ray_facing_up = is_ray_facing_up;
  rays[ray_index].is_ray_facing_left = is_ray_facing_left;
  rays[ray_index].is_ray_facing_right = is_ray_facing_right;
}

void cast_all_rays() {
  float ray_angle = player.rotation_angle - FOV_ANGLE / 2;

  for (int ray_index = 0; ray_index < NUM_RAYS; ray_index++) {
    cast_ray(ray_angle, ray_index);
    ray_angle += FOV_ANGLE / NUM_RAYS;
  }
}

void generate_wall_projection() {
  for (int x = 0; x < NUM_RAYS; x++) {
    float ray_distance = rays[x].distance * cos(rays[x].angle - player.rotation_angle);
    float distance_to_projection_plane = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);
    float projected_wall_height = (TILE_SIZE / ray_distance) * distance_to_projection_plane;

    int wall_height = (int)projected_wall_height;

    int wall_top_pixel = (WINDOW_HEIGHT / 2) - (wall_height / 2);
    wall_top_pixel = wall_top_pixel < 0 ? 0 : wall_top_pixel;

    int wall_bottom_pixel = (WINDOW_HEIGHT / 2) + (wall_height / 2);
    wall_bottom_pixel = wall_bottom_pixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wall_bottom_pixel;

    for (int y = 0; y < wall_top_pixel; y++) {
      sys.color_buffer[(WINDOW_WIDTH * y) + x] =  0xFFAAAAAA;
    }

    for (int y = wall_top_pixel; y < wall_bottom_pixel; y++) {
      sys.color_buffer[(WINDOW_WIDTH * y) + x] = rays[x].was_hit_vert ? 0xFFFFFFFF : 0xFFCCCCCC;
    }

    for (int y = wall_bottom_pixel; y < WINDOW_HEIGHT; y++) {
      sys.color_buffer[(WINDOW_WIDTH * y) + x] =  0xFF333333;
    } 
  }
}

void render_rays() {
  SDL_SetRenderDrawColor(sys.renderer, 0, 204, 102, 255);
  for (int ray_index = 0; ray_index < NUM_RAYS; ray_index++) {
    SDL_RenderDrawLine(
      sys.renderer,
      player.x * MINIMAP_SCALE_FACTOR,
      player.y * MINIMAP_SCALE_FACTOR,
      rays[ray_index].wall_hit_x * MINIMAP_SCALE_FACTOR,
      rays[ray_index].wall_hit_y * MINIMAP_SCALE_FACTOR
    );
  }
}

void on_key_down(SDL_Keycode code) {
  switch (code) {
  case SDLK_ESCAPE:
    sys.quit = TRUE;
    break;
  case SDLK_UP:
    player.walk_direction = +1;
    break;
  case SDLK_DOWN:
    player.walk_direction = -1;
    break;
  case SDLK_RIGHT:
    player.turn_direction = +1;;
    break;
  case SDLK_LEFT:
    player.turn_direction = -1;
    break;
  default:
    break;
  }
}

void on_key_up(SDL_Keycode code) {
  switch (code) {
  case SDLK_UP:
    player.walk_direction = 0;
    break;
  case SDLK_DOWN:
    player.walk_direction = 0;
    break;
  case SDLK_RIGHT:
    player.turn_direction = 0;
    break;
  case SDLK_LEFT:
    player.turn_direction = 0;
    break;
  default:
    break;
  }
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    sys.quit = TRUE;
    break;
  case SDL_KEYDOWN:
    on_key_down(event.key.keysym.sym);
    break;
  case SDL_KEYUP:
    on_key_up(event.key.keysym.sym);
    break;
  default:
    break;
  }
}

void setup() {
  player.x = WINDOW_WIDTH / 2;
  player.y = WINDOW_HEIGHT / 2;
  player.width = 10;
  player.height = 10;
  player.turn_direction = 0;
  player.walk_direction = 0;
  player.rotation_angle = PI / 2;
  player.walk_velocity = 130;
  player.turn_velocity = 120 * (PI / 180);

  sys.color_buffer = (Uint32*)malloc(sizeof(Uint32) * (Uint32)WINDOW_WIDTH * (Uint32)WINDOW_HEIGHT);

  sys.color_buffer_texture = SDL_CreateTexture(
    sys.renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );
};

void update() {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - sys.last_frame_time);

  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  float delta_time = (SDL_GetTicks() - sys.last_frame_time) / 1000.0f;
  sys.last_frame_time = SDL_GetTicks();

  move_player(&player, delta_time);
  cast_all_rays();
};

void render() {
  SDL_SetRenderDrawColor(sys.renderer, 0, 0, 0, 255);
  SDL_RenderClear(sys.renderer);

  generate_wall_projection();
  render_color_buffer();
  clear_color_buffer(0xFF000000);

  // render_map();
  // render_player();
  // render_rays();

  SDL_RenderPresent(sys.renderer);
};

void game_loop() {
  setup();

  while (!sys.quit) {
    process_input();
    update();
    render();
  }
}

int main() {
  initialize_window();
  game_loop();
  destroy_window();

  return 0;
}