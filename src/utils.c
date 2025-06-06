#include "utils.h"

void normalize_angle(float* angle) {
  *angle = remainder(*angle, TWO_PI);
  if (*angle < 0) {
    *angle = TWO_PI + *angle;
  }
}

float distance_between_points(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float lerp(float a, float b, float t) {
  return a + (b - a) * t;
}