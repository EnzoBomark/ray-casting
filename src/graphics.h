#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "defs.h"
#include "font.h"
#include "texture.h"

void initialize_window(void);
void destroy_window(void);
void render_clear(void);
void render_present(void);
void clear_color_buffer(color_t color);
void render_color_buffer(void);
void draw_pixel(int x, int y, color_t color);
void draw_rect(int x, int y, int width, int height, color_t color);
void draw_line(int x0, int y0, int x1, int y1, color_t color);

void render_debug_menu(char* debug_text);
void render_crosshair(void);

#endif
