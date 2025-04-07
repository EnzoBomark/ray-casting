#ifndef WEAPON_H
#define WEAPON_H

#include <SDL2/SDL.h>

#include "defs.h"
#include "upng.h"
#include "game-state.h"
#include "graphics.h"
#include "texture.h"
#include "player.h"

#define NUM_WEAPONS 1

typedef struct {
  int id;
  int ammo;
  int max_ammo;
  int damage;
  float fire_rate;
  float reload_time;
  upng_t* textures[5];
  int current_texture;
} weapon_t;

extern weapon_t weapon;

void load_weapons(void);
void render_weapon(void);
void fire_weapon(void);
void update_weapon(void);

#endif