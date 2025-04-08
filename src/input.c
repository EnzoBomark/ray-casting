#include "input.h"

bool key_down[SDL_NUM_SCANCODES];
bool key_pressed[SDL_NUM_SCANCODES];
bool mouse_button_down[8];
bool mouse_button_pressed[8];
float mouse_motion_rel_x;

void process_input(void) {
  mouse_motion_rel_x = 0.0f;
  memset(key_pressed, 0, sizeof(key_pressed));
  memset(mouse_button_pressed, 0, sizeof(mouse_button_pressed));

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_MOUSEMOTION:
      mouse_motion_rel_x = (event.motion.xrel * 0.05f) * game_state.delta_time;
      break;
    case SDL_KEYDOWN:
      if (!event.key.repeat) {
        key_down[event.key.keysym.scancode] = true;
        key_pressed[event.key.keysym.scancode] = true;
      }
      break;
    case SDL_KEYUP:
      key_down[event.key.keysym.scancode] = false;
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button < 8) {
        mouse_button_down[event.button.button] = true;
        mouse_button_pressed[event.button.button] = true;
      }
      break;
    case SDL_MOUSEBUTTONUP:
      if (event.button.button < 8) {
        mouse_button_down[event.button.button] = false;
      }
      break;
    default:
      break;
    }
  }
}


bool is_key_down(SDL_Keycode code) {
  const SDL_Scancode scancode = SDL_GetScancodeFromKey(code);
  return key_down[scancode];
}

bool is_key_pressed(SDL_Keycode code) {
  const SDL_Scancode scancode = SDL_GetScancodeFromKey(code);
  return key_pressed[scancode];
}

bool is_mouse_button_down(Uint8 button) {
  if (button >= 8) return false;
  return mouse_button_down[button];
}

bool is_mouse_button_pressed(Uint8 button) {
  if (button >= 8) return false;
  return mouse_button_pressed[button];
}

float get_mouse_motion_rel_x(void) {
  return mouse_motion_rel_x;
}