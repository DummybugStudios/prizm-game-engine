#include <engine/drawing.h>
#include <fxcg/display.h>
#include <engine/constants.h> 

// TODO: add color option
void draw_rectangle(Collider *collider)
{
    // start at the top left
    float x = collider->x;
    float y = collider->y;
    float sizey = collider->collider.rect.height;
    float sizex = collider->collider.rect.width;

    int startpoint = (y-sizey)*LCD_WIDTH_PX + (x-sizex); 
    for (int i = startpoint; i < startpoint+sizex; i++)
    { 
        for (int j = 0; j < sizey; j++)
        { 
            // make it green for now 
            vramaddress[i+LCD_WIDTH_PX*j] = 0b0000011111100000;
        }
    }
}

// draw circle outline
void draw_circle (Collider *collider)
{
    int x0 = collider->x;
    int y0 = collider->y;
    int radius = collider->collider.circle.radius;

    int x = radius - 1;
    int y = 0;

    int dx,dy;
    dx = dy = 1;

    int err = dx - (radius << 1);

    int color = 0b0000011111100000;
    while (x >= y)
    {
        vramaddress[x0 + x + (y0 + y) * WIDTH] = color;
        vramaddress[x0 + y + (y0 + x) * WIDTH] = color;
        vramaddress[x0 - y + (y0 + x) * WIDTH] = color;
        vramaddress[x0 - x + (y0 + y) * WIDTH] = color;
        vramaddress[x0 - x + (y0 - y) * WIDTH] = color;
        vramaddress[x0 - y + (y0 - x) * WIDTH] = color;
        vramaddress[x0 + y + (y0 - x) * WIDTH] = color;
        vramaddress[x0 + x + (y0 - y) * WIDTH] = color;

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

void draw_collider(Collider *collider)
{
    switch (collider->type)
    {
        case BOX_COLLIDER:
        draw_rectangle(collider);
        break;

        case CIRCLE_COLLIDER:
        draw_circle(collider);
        break;
    }
}