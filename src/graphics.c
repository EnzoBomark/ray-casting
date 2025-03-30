#include "graphics.h"

typedef struct {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* color_buffer_texture;
  Uint32* color_buffer;
} Graphics;

static Graphics graphics = {
  .renderer = NULL,
  .window = NULL,
  .color_buffer_texture = NULL,
  .color_buffer = NULL,
};

void initialize_window(void) {
  ASSERT(!SDL_Init(SDL_INIT_EVERYTHING), "Error initializing SDL. %s", SDL_GetError());

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  int fullscreen_width = display_mode.w;
  int fullscreen_height = display_mode.h;

  graphics.window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    fullscreen_width,
    fullscreen_height,
    SDL_WINDOW_BORDERLESS
  );

  ASSERT(graphics.window, "Error creating SDL Window. %s", SDL_GetError());

  graphics.renderer = SDL_CreateRenderer(graphics.window, -1, 0);

  ASSERT(graphics.renderer, "Error creating SDL Renderer. %s", SDL_GetError());

  SDL_SetRenderDrawBlendMode(graphics.renderer, SDL_BLENDMODE_BLEND);

  graphics.color_buffer = (color_t*)malloc(sizeof(color_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

  graphics.color_buffer_texture = SDL_CreateTexture(
    graphics.renderer,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING,
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );
}

void destroy_window(void) {
  free(graphics.color_buffer);
  SDL_DestroyTexture(graphics.color_buffer_texture);
  SDL_DestroyRenderer(graphics.renderer);
  SDL_DestroyWindow(graphics.window);
  SDL_Quit();
}

void clear_color_buffer(color_t color) {
  for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
    graphics.color_buffer[i] = color;
  }
}

void render_color_buffer(void) {
  SDL_UpdateTexture(
    graphics.color_buffer_texture,
    NULL,
    graphics.color_buffer,
    (int)(WINDOW_WIDTH * sizeof(color_t))
  );

  SDL_RenderCopy(graphics.renderer, graphics.color_buffer_texture, NULL, NULL);
  SDL_RenderPresent(graphics.renderer);
}

void draw_pixel(int x, int y, color_t color) {
  graphics.color_buffer[(WINDOW_WIDTH * y) + x] = color;
}

void draw_rect(int x, int y, int width, int height, color_t color) {
  for (int i = x; i <= (x + width); i++) {
    for (int j = y; j <= (y + height); j++) {
      draw_pixel(i, j, color);
    }
  }
}

void draw_line(int x0, int y0, int x1, int y1, color_t color) {
  int delta_x = (x1 - x0);
  int delta_y = (y1 - y0);

  int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

  float x_increment = delta_x / (float)longest_side_length;
  float y_increment = delta_y / (float)longest_side_length;

  float current_x = x0;
  float current_y = y0;

  for (int i = 0; i < longest_side_length; i++) {
    draw_pixel(round(current_x), round(current_y), color);
    current_x += x_increment;
    current_y += y_increment;
  }
}