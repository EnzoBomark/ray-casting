#ifndef UTILS_H
#define UTILS_H

#include <limits.h>

#include "defs.h"

void normalize_angle(float* angle);
float distance_between_points(float x1, float y1, float x2, float y2);
float lerp(float a, float b, float t);

#endif
