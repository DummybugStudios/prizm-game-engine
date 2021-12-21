#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

typedef struct Sprite
{ 
    int width;
    int height;
    unsigned char *location; 

} Sprite; 

Sprite *loadSprite(char *path);
void unloadSprite(Sprite **sprite);
void drawSprite(Sprite *sprite); 
int testfunc(int x, int y); 

#endif