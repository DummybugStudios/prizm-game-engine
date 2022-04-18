#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

#include "collider.h"

static inline double clamp(double val, double low, double high)
{
    const double ret = val < low ? low : val;
    return ret > high ? high : ret; 
}

void handle_collision_physics(Collider *a, Collider *b);

bool isIntersecting(Collider *obj1, Collider *obj2);

// CASIO specific utilities

#ifdef __SH4A__
#include <stdio.h>

int key_pressed(int basic_keycode);

void fatal_error(char *fmt, ...);

void debug_print(char *fmt, ...);

void breakpoint();

#ifndef NDEBUG
#define assert(condition) if (!(condition)) fatal_error("Assertion '" #condition "' failed");
#else
#define assert(condition)
#endif

#endif // __SH4A__

#endif