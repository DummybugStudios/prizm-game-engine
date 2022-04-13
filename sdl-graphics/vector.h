//
// Created by sid on 13/04/2022.
//

#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>

typedef struct Vector
{
    float x;
    float y;
} Vector;

static inline Vector vec_multiply(Vector vector, float scalar)
{
    vector.x *= scalar;
    vector.y *= scalar;
    return vector;
}

// Returns the square of the magnitude
static inline float sq_magnitude(Vector vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

// Try not to use this if not needed since sqrt is slow
// Try to use sq_magnitude instead
static inline float magnitude(Vector vector)
{
    return sqrt(sq_magnitude(vector));
}

static inline Vector vec_add(Vector vec1, Vector vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    return vec1;
}

static inline Vector normalize(Vector vector)
{
    return vec_multiply(vector, 1/magnitude(vector));
}

static inline float dot(Vector v1, Vector v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

#endif //VECTOR_H
