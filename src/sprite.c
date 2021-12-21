#include <stdio.h>
#include <stdlib.h>

#include <fxcg/file.h> 
#include <fxcg/display.h>
#include <engine/sprite.h> 
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_STDIO
#define STBI_ONLY_PNG
#include "stb_image.h"

// TODO: Add all the fatal error utilities
Sprite *loadSprite(char *name)
{
    Sprite *sprite = malloc(sizeof(Sprite));

    // strlen does not take null byte into account
    unsigned short *buffer = malloc(strlen(name)*2+2);
    Bfile_StrToName_ncpy(buffer, name, strlen(name)+1); 

    // TODO: check if file openend properly
    int h_file = Bfile_OpenFile_OS(buffer, READ, 0);
    free(buffer); 

    // Get file size and read it
    int size = Bfile_GetFileSize_OS(h_file);
    unsigned char *sprite_png = malloc(size); 
    Bfile_ReadFile_OS(h_file, sprite_png, size, 0); 
    Bfile_CloseFile_OS(h_file);

    int x, y, channels;

    unsigned char *sprite_data = stbi_load_from_memory(sprite_png, size, &x, &y, &channels, 3); 
    free(sprite_png); 

    sprite->width = x;
    sprite->height =y;
    sprite->data = sprite_data;

    return sprite; 
}

void unloadSprite(Sprite **sprite)
{
    free(*sprite);
    // TODO: Who is responsible for setting this to null? 
    *sprite = NULL;
}

void drawSprite(Sprite *sprite)
{ 

    static unsigned short *vramaddress = NULL; 

    __builtin_expect(sprite != NULL,1); 
    if (sprite == NULL)
        return;

    // TODO: ideally we won't waste instructions on this
    __builtin_expect(vramaddress != NULL, 1); 
    if (!vramaddress)
    {
        vramaddress = GetVRAMAddress(); 
    }

    for (int rows = 0; rows < sprite->height; rows ++) {
        // width = 100; 
        for (int cols = 0; cols < sprite->width; cols++)
        {

            // rgb888 colours
            uint8_t red   = sprite->data[cols*3 + rows*3*sprite->width];
            uint8_t green = sprite->data[cols*3 + rows*3*sprite->width+1];
            uint8_t blue  = sprite->data[cols*3 + rows*3*sprite->width+2];

            // convert rgb888 to rgb565 
            uint16_t r = (red >> 3) << 11;
            uint16_t g = (green >> 2) << 5; 
            uint16_t b = blue >> 3;
        
            vramaddress[cols + rows*LCD_WIDTH_PX] = (uint16_t)(r | g | b);
        }
    }
}