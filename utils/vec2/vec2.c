#include "vec2.h"
#include <math.h>
#include <stdio.h>

/// Create a 2d vector.
/// @param x_ The first component.
/// @param y_ The second component.
/// @return The newly created vector.
vec2 vec2_create(double x_, double y_)
{
    return (vec2){x_, y_};
}

/// Create a zero 2d vector.
/// @return The newly created zero vector.
vec2 vec2_create_zero()
{
    return vec2_create(0.0, 0.0);
}

/// Add two vectors.
/// @param lhs The left operand.
/// @param rhs The right operand.
/// @return The sum in a new vector.
vec2 vec2_add(vec2 lhs, vec2 rhs)
{
    return vec2_create(lhs.x + rhs.x, lhs.y + rhs.y);
}

/// Substract two vectors.
/// @param lhs The left operand.
/// @param rhs The right operand.
/// @return The difference in a new vector.
vec2 vec2_sub(vec2 lhs, vec2 rhs)
{
    return vec2_create(lhs.x - rhs.x, lhs.y - rhs.y);
}

/// Multiply a vector by a scalar.
/// @param scalar The left operand, a scalar.
/// @param rhs The right operand, a vector.
/// @return The product in a new vector.
vec2 vec2_mul(double scalar, vec2 rhs)
{
    return vec2_create(rhs.x * scalar, rhs.y * scalar);
}

/// Multiply a vector by an other vector.
/// @param lhs The left operand, a vector.
/// @param rhs The right operand, a vector.
/// @return The product in a new vector.
vec2 vec2_mul_vec(vec2 lhs, vec2 rhs)
{
    return vec2_create(rhs.x * lhs.x, rhs.y * lhs.y);
}

/// Gets the square root of a vector
/// @param vec The vector
/// @return The product in a new vector.
vec2 vec2_sqrt(vec2 vec)
{
    return vec2_create(sqrt(vec.x), sqrt(vec.y));
}

/// Compute the dot product (scalar product) between two vectors.
/// @param lhs The left operand.
/// @param rhs The right operand.
/// @return The dot product.
double vec2_dot(vec2 lhs, vec2 rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

/// Compute the square of the euclidean norm of a given vector.
/// @param v The vector.
/// @return The square of the norm.
double vec2_norm_sqr(vec2 v)
{
    return pow(vec2_norm(v), 2);
}

/// Compute the euclidean norm of a given vector.
/// @param v The vector.
/// @return The norm.
double vec2_norm(vec2 v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

/// Compute the normalization of a given vector.
/// @param v The vector.
/// @return The new normalized vector.
vec2 vec2_normalize(vec2 v)
{
    return vec2_create(v.x / vec2_norm(v), v.y / vec2_norm(v));
}

/// Check whether two vectors are approximately equals within a given tolerance.
/// @param lhs The left operand.
/// @param rhs The right operand.
/// @param eps The tolerance.
/// @return true if vector are approximately equal, false otherwise.
bool vec2_is_approx_equal(vec2 lhs, vec2 rhs, double eps)
{
    return fabs(lhs.x - rhs.x) <= eps && fabs(lhs.y - rhs.y) <= eps;
}

/// Compute the coordinates of a 2d vector (with components between -1 and 1)
/// in a given screen matrix.
/// @param v The 2d vector.
/// @param width The screen width.
/// @param height The screen height.
/// @return The coordinates (row, column).
coordinates vec2_to_coordinates(vec2 v, uint32_t width, uint32_t height)
{
    coordinates result;
    result.column = ((v.x + 1) / 2 * (width - 1)) + 0.5;

    result.row = ((v.y + 1) / 2 * (height - 1)) + 0.5;
    return result;
}

/// Print a vector in the standard output.
/// @param v The vector.
void vec2_print(vec2 v)
{
    printf("x = %g, y = %g\n", v.x, v.y);
}
