#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdint.h>
#include "./vec2/vec2.h"
#include "./gfx/gfx.h"
#include "./charge/charge.h"

coordinates_t coordinates_create(int row_, int column_);

// Transform a position in the univers [x0,y0]x[x1,y1] to a screen position
coordinates_t position_to_coordinates(int width, int height, double x0, double x1, double y0, double y1, vec2 pos);

double rand_one();

#endif