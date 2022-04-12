#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

#include "collider.h"

static inline double clamp(double val, double low, double high)
{
    const double ret = val < low ? low : val;
    return ret > high ? high : ret; 
}

bool isIntersecting(Collider *obj1, Collider *obj2);

#endif