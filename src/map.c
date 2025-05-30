#include "map.h"

static const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 2, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5}
};

bool has_map_wall_at(float x, float y) {
  if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
    return true;
  }

  int x_gird_index = floor(x / TILE_SIZE);
  int y_gird_index = floor(y / TILE_SIZE);
  return map[y_gird_index][x_gird_index] != 0;
}

int get_map_wall_at(float x, float y) {
  if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
    return true;
  }

  int x_gird_index = floor(x / TILE_SIZE);
  int y_gird_index = floor(y / TILE_SIZE);
  return map[y_gird_index][x_gird_index];
}

bool is_inside_map(float x, float y) {
  return (x >= 0 && x <= MAP_NUM_COLS * TILE_SIZE && y >= 0 && y <= MAP_NUM_ROWS * TILE_SIZE);
}

void render_map_grid() {
  for (int y = 0; y < MAP_NUM_ROWS; y++) {
    for (int x = 0; x < MAP_NUM_COLS; x++) {
      int tile_x = x * TILE_SIZE;
      int tile_y = y * TILE_SIZE;
      color_t tile_color = map[y][x] != 0 ? 0xFFFFFFFF : 0xFF000000;
      draw_rect(
        (tile_x * MINIMAP_SCALE_FACTOR) + MINIMAP_X_OFFSET,
        (tile_y * MINIMAP_SCALE_FACTOR) + MINIMAP_Y_OFFSET,
        TILE_SIZE * MINIMAP_SCALE_FACTOR,
        TILE_SIZE * MINIMAP_SCALE_FACTOR,
        tile_color
      );
    }
  }
}