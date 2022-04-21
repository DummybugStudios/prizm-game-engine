#ifdef __SH4A__
#include <stdlib.h>
#include <engine/drawing.h>
#include <fxcg/display.h>
#include <engine/constants.h>
#include <engine/utils.h>

void draw_rectangle_filled(int x, int y, int width, int height, int color)
{
    int startpoint = x + y*LCD_WIDTH_PX; 
    for (int i = startpoint; i < startpoint+width; i++)
    { 
        for (int j = 0; j < height; j++)
        {
            putpixel(i,j,color); 
        }
    }
}

// draw circle outline
void draw_circle (int x0, int y0, int radius,int color)
{
    int x = radius - 1;
    int y = 0;

    int dx,dy;
    dx = dy = 1;

    int err = dx - (radius << 1);

    while (x >= y)
    {
        putpixel(x0 + x, y0 + y, color);
        putpixel(x0 + y, y0 + x, color);
        putpixel(x0 - y, y0 + x, color);
        putpixel(x0 - x, y0 + y, color);
        putpixel(x0 - x, y0 - y, color);
        putpixel(x0 - y, y0 - x, color);
        putpixel(x0 + y, y0 - x, color);
        putpixel(x0 + x, y0 - y, color);


        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }

}

void draw_circle_filled(int x0, int y0, int radius, int color)
{
    int radius_sqr = radius*radius;
    for (int x = -radius; x < 1; x++)
    {
        int height = (int) sqrt(radius_sqr - x*x);
        for (int y = -height; y < 1; y++) 
        {
            putpixel(x0+x, y0+y, color);
            putpixel(x0+x, y0-y, color);
            putpixel(x0-x, y0+y, color);
            putpixel(x0-x, y0-y, color);
        }
    }
}

void draw_collider(Collider *collider)
{
    switch (collider->type)
    {
        case BOX_COLLIDER:
        draw_rectangle_filled(
            collider->x,
            collider->y,
            collider->collider.rect.width,
            collider->collider.rect.height,
            rgb565(collider->r, collider->g, collider->b)
        );

        break;

        case CIRCLE_COLLIDER:
        draw_circle_filled(
            collider->x,
            collider->y,
            collider->collider.circle.radius,
            rgb565(collider->r, collider->g, collider->b)
        );
        break;
    }
}

void draw_line(int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        putpixel(x0,y0,color);
        if (x0 == x1 && y0 == y1) break; 

        int e2 = err << 1;

        if (e2 >= dy)
        {
            if (x0 == x1) break;
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            if (y0 == y1) break;
            err += dx;
            y0 += sy; 
        }
    }

}

#endif // __SH4A__