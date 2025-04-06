
#ifndef FONT_H
#define FONT_H

#include <stdio.h>
#include "defs.h"
#include <SDL2/SDL_ttf.h>

extern TTF_Font* font;

void load_font(void);
void free_font(void);

#endif
