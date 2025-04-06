#include "font.h"

TTF_Font* font = NULL;

void load_font(void) {
  ASSERT(!TTF_Init(), "Error initializing TTF. %s", SDL_GetError());

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  int fullscreen_height = display_mode.h;

  int font_size = fullscreen_height / 50;

  font = TTF_OpenFont("./assets/fonts/dogica.ttf", font_size);

  ASSERT(font, "Error creating TTF Font. %s", TTF_GetError());
}

void free_font(void) {
  // TTF_CloseFont(font);
  // font = NULL;
}
