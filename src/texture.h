
#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdint.h>
#include "defs.h"
#include "upng.h"

#define NUM_TEXTURES 18

upng_t* textures[NUM_TEXTURES];

void load_textures(void);
void free_textures(void);

#endif
