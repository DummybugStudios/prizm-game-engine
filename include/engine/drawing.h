// TODO: provide drawing functions for SDL applications as well
#ifdef __SH4A__

#ifndef ENGINE_DRAWING_H
#define ENGINE_DRAWING_H

#include <engine/collider.h>
#include <engine/constants.h>

static inline void putpixel(int x, int y, int color)
{
    vramaddress[x + WIDTH *y] = color;
}

void draw_circle (int x0, int y0, int radius, int color);
void draw_circle_filled(int x0, int y0, int radius, int color);
void draw_rectangle_filled(int x, int y, int width, int height, int color);
void draw_line(int x1, int y1, int x2, int y2, int color);
void draw_collider (Collider *collider);

#endif // ENGINE_DRAWING_H

#endif // __SH4A__