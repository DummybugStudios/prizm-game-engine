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
    float x = collider->x;
    float y = collider->y;
    float radius = collider->collider.circle.radius;
    
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