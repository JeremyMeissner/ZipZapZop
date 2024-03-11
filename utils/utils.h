#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdint.h>

const double K = 8.988e9;
const double E = 1.602e-19;

typedef struct
{
    uint32_t row;
    uint32_t column;
} coordinates_t;

typedef struct
{
    double q;
    vec2 pos;
} charge_t;

coordinates_t coordinates_create(int row_, int column_);

// Transform a position in the univers [x0,y0]x[x1,y1] to a screen position
coordinates_t position_to_coordinates(int width, int height, double x0, double x1, double y0, double y1, vec2 pos);

double rand_one();

charge_t charge_create(double q, vec2 pos);

#endif