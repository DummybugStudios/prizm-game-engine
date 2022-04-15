#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h> 
#include <fxcg/file.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#include <engine/sprite.h>
#include <engine/hgrid.h>
#include <engine/collider.h>
#include <engine/constants.h>
#include <engine/drawing.h>
#include <engine/utils.h>
#include <engine/vector.h>

int main(void){
    init_engine();

    Bdisp_EnableColor(1);
    sys_srand(RTC_GetTicks());

    // Create colliders
    Collider colliders[OBJECTS];
    for (int i = 0; i < OBJECTS; i++)
    {
        // colliders[i].type = BOX_COLLIDER;
        // colliders[i].collider.rect.width    = 
        // colliders[i].collider.rect.height   = 20;

        colliders[i].type = CIRCLE_COLLIDER;
        colliders[i].collider.circle.radius = 10;

        colliders[i].x = sys_rand() % LCD_WIDTH_PX - 20;
        colliders[i].x = colliders[i].x < 0 ? 0 : colliders[i].x;

        colliders[i].y = sys_rand() % LCD_HEIGHT_PX - 20;
        colliders[i].y = colliders[i].y < 0 ? 0 : colliders[i].y; 
    }
    
    for(;;){
        
        Bdisp_AllClr_VRAM();
        for (int i =0; i < sizeof(colliders) /sizeof(Collider); i++)
        { 
            draw_collider(&colliders[i]);
        }

        // TODO: make print debugging a utility function
        // char debug[255];
        // sprintf(debug, "%f",Q_rsqrt(0.8f));
        // locate_OS(1,1);
        // Print_OS(debug, 0,0);

        
        if(key_pressed(KEY_PRGM_MENU)){
            int key;
            GetKey(&key);
            break;
        }
        Bdisp_PutDisp_DD();
    }

    return 0;
}
