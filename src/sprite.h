#ifndef SPRITE_H
#define SPRITE_H

#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "texture.h"
#include "utils.h"
#include "ray.h"

typedef struct {
  float x;
  float y;
  float distance;
  float angle;
  bool visible;
  int texture;
} sprite_t;

void render_sprite_projection(void);
void render_map_sprites(void);

#endif