#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>
#include <math.h>

#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); exit(1); }

#define PI 3.14159265
#define TWO_PI 6.28318530

#define TILE_SIZE 64

#define MINIMAP_SCALE_FACTOR 0.3

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

#define FOV_ANGLE (90 * (PI / 180))
#define NUM_RAYS WINDOW_WIDTH

#define DIST_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))

#define FPS 120
#define FRAME_TARGET_TIME (1000 / FPS)

typedef uint32_t color_t;

#endif