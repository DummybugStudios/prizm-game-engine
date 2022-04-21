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
#include <stdint-gcc.h>

// converts rgb-888 to rgb565
static inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t red, green, blue;

    red = (r >> 3) << 11;
    green = (g >> 2) << 5;
    blue = b >> 3;

    return red | green | blue;
}

int key_pressed(int basic_keycode);

void fatal_error(char *fmt, ...);

void debug_print(char *fmt, ...);

void breakpoint(char *name);

// shady sqrt method i found on the internet
static inline float sqrt(float num)
{
    const int result = 0x1fbb4000 + (*(int*)&num >> 1);
    return *(float*)&result;
}

#ifndef NDEBUG
#define assert(condition) if (!(condition)) fatal_error("Assertion '" #condition "' failed");
#else
#define assert(condition)
#endif // NDEBUG


# else // avoid issues in the SDL-Graphics project
static inline void breakpoint(char *name){};
#endif // __SH4A__


#endif