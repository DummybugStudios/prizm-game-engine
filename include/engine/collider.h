#ifndef OBJECTS_H
#define OBJECTS_H

#include <sys/types.h>
#include "linked_list.h"
#include <stdbool.h>

typedef enum ColliderType
{
    BOX_COLLIDER,
    CIRCLE_COLLIDER
} ColliderType;

typedef enum PhsyicsType
{
    STATIC,
    DYNAMIC,
    TRIGGER
} PhsyicsType;

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
    PhsyicsType physics;

    int r;
    int g;
    int b;
    
    int tag;    // custom tag that the user can put in here
    int level;  // for use by hash grid
    float vx;
    float vy;
    float x;
    float y;
    bool isColliding;
    bool isDisabled;

    union collider {
        BoxCollider rect;
        CircleCollider circle;
    } collider;

} Collider;


// basically a worse implementation of std::vector
typedef struct ColliderCollection
{
    unsigned int capacity;
    unsigned int size;
    Collider *data;
} ColliderCollection;

// A single set of colliders for the whole game
// Maybe not the best solution but API gets more complicated the more flexible I try to make it
// But this can be easily changed in the future
extern  ColliderCollection colliders;

void create_colliders(unsigned int size);

int add_collider(Collider collider);

void disable_collider(unsigned int n);

void enable_collider(unsigned int n);

// If you want to make more specific changes to the collider.
Collider *get_collider(unsigned int n );

// Should I provide direct access to the variables? Probably not.

// Get capacity that was set at the start
unsigned int get_colliders_limit();

// Get the number of objects that are in there right now
unsigned int get_colliders_size();

#endif