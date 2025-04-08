
#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "game-state.h"

void process_input(void);
bool is_key_down(SDL_Keycode code);
bool is_key_pressed(SDL_Keycode code);
bool is_mouse_button_down(Uint8 button);
bool is_mouse_button_pressed(Uint8 button);
float get_mouse_motion_rel_x(void);

#endif
