// TODO: provide drawing functions for SDL applications as well
#ifdef __SH4A__

#ifndef ENGINE_DRAWING_H
#define ENGINE_DRAWING_H

#include <engine/collider.h>

void draw_circle (Collider *collider);
void draw_rectangle (Collider *collider);
void draw_collider (Collider *collider); 

#endif // ENGINE_DRAWING_H

#endif // __SH4A__