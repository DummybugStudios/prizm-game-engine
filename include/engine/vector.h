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

static inline float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
	return y;
}

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
    return sqrt((double)sq_magnitude(vector));
}

static inline Vector vec_add(Vector vec1, Vector vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    return vec1;
}

static inline Vector normalize(Vector vector)
{
    #ifdef __SH4A__
    return vec_multiply(vector, Q_rsqrt(sq_magnitude(vector)));
    #else
    return vec_multiply(vector, 1/magnitude(vector));
    #endif
}

static inline float dot(Vector v1, Vector v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

#endif //VECTOR_H
