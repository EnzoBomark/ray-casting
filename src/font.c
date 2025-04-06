#include "font.h"

TTF_Font* font = NULL;

void load_font(void) {
  ASSERT(!TTF_Init(), "Error initializing TTF. %s", SDL_GetError());

  font = TTF_OpenFont("./assets/fonts/dogica.ttf", 24);

  ASSERT(font, "Error creating TTF Font. %s", TTF_GetError());
}

void free_font(void) {
  // TTF_CloseFont(font);
  // font = NULL;
}
