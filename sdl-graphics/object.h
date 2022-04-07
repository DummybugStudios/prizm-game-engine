#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL2/SDL.h>
#include <sys/types.h>
#include "linked_list.h"
#include <stdbool.h>

typedef struct Object
{
    list_head list;

    SDL_Rect rect;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    float vx;
    float vy;
    float posx;
    float posy;
    bool isColliding;

} Object;


#endif