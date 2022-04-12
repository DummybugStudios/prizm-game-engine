#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL2/SDL.h>
#include <sys/types.h>
#include "linked_list.h"
#include <stdbool.h>

typedef enum ColliderType
{
    BOX_COLLIDER,
    CIRCLE_COLLIDER
} ColliderType;

typedef struct BoxCollider {
    float width;
    float height;
} BoxCollider;

typedef struct CircleCollider {
    float radius;
} CircleCollider;

typedef struct Collider
{
    list_head list;
    ColliderType type;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    int level;  // for use by hash grid
    float vx;
    float vy;
    float x;
    float y;
    bool isColliding;

    union collider {
        BoxCollider rect;
        CircleCollider circle;
    } collider;

} Collider;


#endif