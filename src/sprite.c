#include <stdio.h>
#include <stdlib.h>

#include <fxcg/file.h> 
#include <fxcg/display.h>
#include <engine/sprite.h> 

Sprite *loadSprite(char *name)
{
    Sprite *sprite = malloc(sizeof(Sprite));
    sprite->height = 100;
    sprite->width = 40;
    sprite->location = NULL; 
    return sprite; 
}

void unloadSprite(Sprite **sprite)
{
    *sprite = NULL;
}

void drawSprite(Sprite *sprite)
{ 
    int x = 10;
    int y = 10;
    x++;
    y++; 
    char *message = "Hello world";
    PrintMini(&x, &y, message, 0x02, -1,0,0,1,0,1,0);
}

int testfunc(int x, int y)
{ 
    return x + y; 
}