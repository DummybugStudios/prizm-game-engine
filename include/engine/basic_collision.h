#ifndef ENGINE_BASIC_COLLISION_H
#define ENGINE_BASIC_COLLISION_H

#include <engine/collider.h>

void detect_basic_collision(Collider *objects, void (* callback)(Collider *, Collider *));

#endif