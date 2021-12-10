#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h> 
#include <string.h>
#include <stdio.h>
#include "images.h"
/* Note that a progress bar has the side effect of making the long calculation you are doing to take longer.
 * The reason for this is because the screen needs to be redrawn.
 * Try figuring out an optimal balance between progress bar responsiveness and how many times it is updated.
 * A few times a second should be good enough.
 * This example does not do any long calculations like a real program would.
 * Instead it just redraws the progress bar a lot to make it take awhile thus demonstrating my point about limiting how often to redraw the progress bar.
 * Also consider: "Why do I need a progress bar? Can my code be written to be faster removing the need for a progress bar?".
 * Really the only reason you would need a progress bar is if the calcualtion takes lots of time.
 * Two seconds as an example without a progress bar should be fine.
 * 
 * If the calculation takes long enough to make the program go unresponsive for a noticeable while (e.g. one or two seconds), but not enough to warrant
 * showing a progress bar, or if you don't have a way to estimate when the calculation will be done, consider calling the syscall HourGlass periodically.
 * It will show the OS busy indicator in the top-right corner of the screen, and will not affect the speed of the program as much, since it only
 * redraws a small part of the screen. This way, it becomes apparent to the user that your program has not stopped working, and that the user should wait.
 */


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


void drawSprite() {
    // width = 100 
    for (int i = 0; i < spr_Play.width; i++)
    { 
        // height = 40
        for (int j=0;j < spr_Play.height; j++)
        {
            vramaddress[i+j*LCD_WIDTH_PX] = spr_Play.image[i+j*100];
        }
    }
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
    for(;;){

        Bdisp_AllClr_VRAM();
        // int xxx = 10;
        // int yyy = 10; 
        drawSprite(); 
        drawRectangle(x,y,20,20);

        // char debug[10]; 
        // sprintf(&debug, "delta: %d", delta); 
        // PrintMini(&xxx, &yyy,debug,0x02,-1,0,0,1,0,1,0);

        if(keyPressed(KEY_PRGM_MENU)){
            int key;
            GetKey(&key);
            break;
        }
 
        handleMovement(&x, &y);
        enableGravity(&x, &y); 

        MsgBoxPop();
        Bdisp_PutDisp_DD();
    }

    return 0;
}
