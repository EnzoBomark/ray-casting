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

  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_ShowCursor(SDL_DISABLE);
}

void destroy_window(void) {
  free(graphics.color_buffer);
  free_textures();
  free_font();
  SDL_DestroyTexture(graphics.color_buffer_texture);
  SDL_DestroyRenderer(graphics.renderer);
  SDL_DestroyWindow(graphics.window);
  SDL_Quit();
}

void render_clear(void) {
  SDL_SetRenderDrawColor(graphics.renderer, 0, 255, 0, 255);
  SDL_RenderClear(graphics.renderer);
}

void render_present(void) {
  SDL_RenderPresent(graphics.renderer);
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

void render_debug_menu(char* debug_text) {
  SDL_Color text_color = { 255, 255, 255, 255 };

  SDL_Surface* text_surface = TTF_RenderText_Solid(font, debug_text, text_color);
  ASSERT(text_surface, "Failed to create text surface: %s\n", TTF_GetError());

  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(graphics.renderer, text_surface);
  ASSERT(text_texture, "Failed to create text texture: %s\n", SDL_GetError());


  int text_width = text_surface->w;
  int text_height = text_surface->h;

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  int fullscreen_width = display_mode.w;

  SDL_Rect text_rect = {
      (fullscreen_width / 2) - (text_width / 2),
      100,
      text_width,
      text_height
  };

  SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 128);
  SDL_Rect background_rect = {
      (fullscreen_width / 2) - ((text_width + 32) / 2),
      100 - 16,
      text_width + 32,
      text_height + 32
  };
  SDL_RenderFillRect(graphics.renderer, &background_rect);
  SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(graphics.renderer, &background_rect);

  SDL_RenderCopy(graphics.renderer, text_texture, NULL, &text_rect);
  SDL_FreeSurface(text_surface);
  SDL_DestroyTexture(text_texture);
}

void render_crosshair(void) {
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  int fullscreen_width = display_mode.w;
  int fullscreen_height = display_mode.h;

  int crosshair_size = 5;
  int crosshair_thickness = 2;

  int center_x = fullscreen_width / 2;
  int center_y = fullscreen_height / 2;

  SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 255, 255);
  SDL_Rect horizontal_line = { center_x - crosshair_size, center_y - (crosshair_thickness / 2), crosshair_size * 2, crosshair_thickness };
  SDL_RenderFillRect(graphics.renderer, &horizontal_line);
  SDL_Rect vertical_line = { center_x - (crosshair_thickness / 2), center_y - crosshair_size, crosshair_thickness, crosshair_size * 2 };
  SDL_RenderFillRect(graphics.renderer, &vertical_line);
}