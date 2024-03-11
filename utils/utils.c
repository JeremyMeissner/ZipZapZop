#include "vec2/vec2.h"
#include "utils.h"
#include <math.h>
#include <stdlib.h>

coordinates_t coordinates_create(int row_, int column_)
{
    coordinates_t c = {.row = row_, .column = column_};
    return c;
}

// Transform a position in the univers [x0,y0]x[x1,y1] to a screen position
coordinates_t position_to_coordinates(int width, int height, double x0, double x1, double y0, double y1, vec2 pos)
{
    double dx = x1 - x0;
    double dy = y1 - y0;
    return coordinates_create((int)round(height * (pos.y - y0) / dy), (int)round(width * (pos.x - x0) / dx));
}

double rand_one()
{
    return (double)rand() / (double)RAND_MAX;
}

charge_t charge_create(double q, vec2 pos)
{
    charge_t c = {.q = q, .pos = pos};
    return c;
}