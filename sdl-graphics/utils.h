#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

#include "object.h"

static inline double clamp(double val, double low, double high)
{
    const double ret = val < low ? low : val;
    return ret > high ? high : ret; 
}

bool isIntersecting(Object *obj1, Object *obj2);

#endif