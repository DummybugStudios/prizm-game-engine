#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h> 
#include <fxcg/file.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#include <engine/hgrid.h>
#include <engine/collider.h>
#include <engine/constants.h>
#include <engine/drawing.h>
#include <engine/utils.h>

void update_objects(Collider *colliders)
{
    //TODO: What order should we do this in? 
    for (int i = 0; i < OBJECTS; i++)
    {
        colliders[i].x += colliders[i].vx;
        colliders[i].y += colliders[i].vy; 
        int radius = colliders[i].collider.circle.radius;

        if (colliders[i].x < radius || colliders[i].x > WIDTH - radius)
        {
            colliders[i].x = colliders[i].x < radius ? radius : WIDTH - radius-1;
            colliders[i].vx *= -1;
        }
        if (colliders[i].y < radius || colliders[i].y > HEIGHT - radius)
        {
            colliders[i].y = colliders[i].y < radius ? radius : HEIGHT - radius-1;
            colliders[i].vy *= -1;
        }
    }
    detect_hgrid_collision(colliders);
}

int main(void){
    init_engine();
    init_hgrid();

    Bdisp_EnableColor(1);
    sys_srand(RTC_GetTicks());

    // Create colliders
    // FIXME: improve collider creation PLEASE
    Collider colliders[OBJECTS];

    // cue ball
    colliders[0].type = CIRCLE_COLLIDER;
    colliders[0].collider.circle.radius = 10;
    colliders[0].x = colliders[0].collider.circle.radius*2*((OBJECTS-1)>>1);
    colliders[0].y = HEIGHT >> 2;
    colliders[0].vy = -3.0f;
    colliders[0].list.next = &colliders[0].list;
    colliders[0].list.prev = &colliders[0].list;

    add_to_hgrid(&colliders[0]);
    // rest of the balls
    for (int i = 1; i < OBJECTS; i++)
    {
        colliders[i].type = CIRCLE_COLLIDER;
        colliders[i].collider.circle.radius = 10;
        colliders[i].level = 0;

        colliders[i].x = 20*i + 5;
        // colliders[i].x = colliders[i].x < 0 ? 0 : colliders[i].x;

        colliders[i].y =10;
        // colliders[i].y = colliders[i].y < 0 ? 0 : colliders[i].y; 

        // initialize colliders!
        colliders[i].list.next = &colliders[i].list;
        colliders[i].list.prev = &colliders[i].list;
        add_to_hgrid(&colliders[i]);
    }
    
    for(;;){
        Bdisp_AllClr_VRAM();
        update_objects(colliders);
        for (int i =0; i < OBJECTS; i++)
        {
            draw_collider(&colliders[i]);
        }
        if(key_pressed(KEY_PRGM_MENU)){
            int key;
            GetKey(&key);
            break;
        }
        Bdisp_PutDisp_DD();
    }

    return 0;
}
