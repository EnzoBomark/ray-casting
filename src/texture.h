
#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdint.h>
#include "defs.h"
#include "upng.h"

#define NUM_TEXTURES 8

typedef struct {
    int width;
    int height;
    color_t* texture_buffer;
    upng_t* upng_texture;
} texture_t;

texture_t wall_textures[NUM_TEXTURES];

void load_wall_textures(void);
void free_wall_textures(void);

#endif
