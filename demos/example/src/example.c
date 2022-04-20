#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h> 
#include <fxcg/file.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#include <engine/basic_collision.h>
#include <engine/uniform_grid_collision.h>
#include <engine/collider.h>
#include <engine/constants.h>
#include <engine/drawing.h>
#include <engine/utils.h>

int delta;
void update_objects(Collider *colliders)
{
    //TODO: What order should we do this in? 

    int currentTime = RTC_GetTicks();
    delta = currentTime -last_tick;
    last_tick = currentTime;

    float friction = 5e-4;
    for (int i = 0; i < OBJECTS; i++)
    {
        colliders[i].x += delta * colliders[i].vx;
        colliders[i].y += delta * colliders[i].vy; 

        colliders[i].vx -= delta*friction*colliders[i].vx;
        colliders[i].vy -= delta*friction*colliders[i].vy;

        if (fabs(colliders[i].vx) < 1e-2) colliders[i].vx = 0;
        if (fabs(colliders[i].vy) < 1e-2) colliders[i].vx =0;
        

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
    detect_uniform_grid_collision(colliders);   
}

int main(void){
    init_engine();

    Bdisp_EnableColor(1);
    sys_srand(RTC_GetTicks());

    // Create balls
    // FIXME: improve collider creation PLEASE
    Collider colliders[OBJECTS];

    int radius = 10;
    // cue ball
    Collider cueball = {
        .list = LIST_HEAD_INIT(cueball.list),
        .type = CIRCLE_COLLIDER,
        .physics = DYNAMIC,
        .vx = 1, .vy = 0.1,
        .x = 20, .y = HEIGHT /4,
        .collider.circle = {
            .radius = radius
        }
    };
    colliders[0] = cueball;

    // place the balls
    int start_y = HEIGHT / 2;
    int start_x = 200;
    float eps = 3;
    for (int level = 1,i=1; i < OBJECTS; level++)
    {
        for (int y = level; y < level+level; y++,i++)
        {
            Collider ball = {
                .list = LIST_HEAD_INIT(ball.list),
                .physics = DYNAMIC,
                .type = CIRCLE_COLLIDER,
                .collider.circle.radius = radius,
                // .vx = 0, .vy = 0,
                .x = start_x + level*(radius*2 + eps),

                .y = start_y                      // position of the center ball
                    - (radius + eps)   * (level-1)    // go up by by radius * level
                    + (radius*2 + eps) * (y - level)  // go down by the diameter depending on ball no.

            };
            colliders[i] = ball;
        }
    };



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
