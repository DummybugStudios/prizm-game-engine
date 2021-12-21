#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h> 
#include <fxcg/file.h>
#include <string.h>
#include <stdio.h>
#include <engine/sprite.h>

#include <math.h>

color_t *vramaddress;

static int keyPressed(int basic_keycode){
    const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
    int row, col, word, bit;
    row = basic_keycode%10;
    col = basic_keycode/10-1;
    word = row>>1;
    bit = col + ((row&1)<<3);
    return (0 != (keyboard_register[word] & 1<<bit));
}

void fatal_error(char *message)
{
    Bdisp_AllClr_VRAM();
    int x = 0;
    int y = 0; 
    PrintMini(&x, &y, message, 0x02, -1,0,0,1,0,1,0);
    for (;;)
    { 
        if (keyPressed(KEY_PRGM_MENU))
        { 
            int key;
            GetKey(&key);
            break;
        }
        Bdisp_PutDisp_DD(); 
    }
}


// TODO: add color option
void drawRectangle(int x, int y, int sizex, int sizey)
{
    // start at the top left
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

static float uy = 0; 
void handleMovement(int *x, int *y)
{ 
        if(keyPressed(KEY_PRGM_LEFT))
            *x--; 
        if(keyPressed(KEY_PRGM_RIGHT))
            *x++; 
        if(keyPressed(KEY_PRGM_UP))
            uy-=0.2; 
        if(keyPressed(KEY_PRGM_DOWN))
            uy+=0.2; 
}


static int lastTick = 0; 
static int delta = 0;
// TODO: Decide how to store multiple objects 
void enableGravity(int *x, int *y)
{
    // pixels per tick squared
    static const float g = 0.1; 
    static float ux = 0;
    static float realy = -1; 
    if (realy == -1)
    {
        realy = (float)(*y);
    }

    // TODO: doesn't work at midnight hahaha
    int newTick = RTC_GetTicks();
    delta = newTick - lastTick; 

    // calculate new position based on u 
    realy += uy*delta;
    if (realy > LCD_HEIGHT_PX)
    {
        // make object bounce when it collides with the bottom of the screen
        realy = LCD_HEIGHT_PX;
        uy = -uy*0.95;
    }
    else if (realy < 0)
    { 
        realy = 0;
        uy = -uy * 0.95;
    }
    else
    {
        *y = (int)realy;
        // update u according to v = u+at
        uy += g*delta;
    }
    lastTick = newTick;
}

int main(void){
    Bdisp_EnableColor(1);

    // initialize the global vram address 
    vramaddress = (color_t *) GetVRAMAddress();
    // initialize position of cube 
    int x = 50;
    int y = 50; 

    // initialize lastTick
    lastTick = RTC_GetTicks(); 
    Sprite *sprite = loadSprite("\\\\fls0\\example_assets\\play.png");
    for(;;){

        Bdisp_AllClr_VRAM();
        // drawSprite(); 
        drawSprite(sprite); 
        drawRectangle(x,y,20,20);

        // int xxx = 1;
        // int yyy = 20; 
        // char debug[70]; 
        // sprintf(&debug, "res: %d",testfunc(2,3)); 
        // char *debug = "Hello";
        // PrintMini(&xxx, &yyy,debug,0x02,-1,0,0,1,0,1,0);

        if(keyPressed(KEY_PRGM_MENU)){
            int key;
            GetKey(&key);
            break;
        }
 
        handleMovement(&x, &y);
        enableGravity(&x, &y); 

        Bdisp_PutDisp_DD();
    }

    return 0;
}
