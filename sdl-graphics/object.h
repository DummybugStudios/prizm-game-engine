#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL2/SDL.h>
#include <sys/types.h>
#include "linked_list.h"
#include <stdbool.h>

// TODO: Find a way to represent this independently of the method being used.
typedef struct Object
{
    list_head list;

    SDL_Rect rect;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    int level;  // for use by hash grid
    int bucket; // for use by hash grid
    float vx;
    float vy;
    float posx;
    float posy;
    bool isColliding;

} Object;


#endif