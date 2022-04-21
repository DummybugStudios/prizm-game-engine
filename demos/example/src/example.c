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
#include <engine/hgrid.h>

#include <engine/collider.h>
#include <engine/constants.h>
#include <engine/drawing.h>
#include <engine/utils.h>


int points = 0;
void callback (Collider *a, Collider *b)
{
    points++;
}

enum Tag {
    DEFAULT, // it's empty by default;
    CUEBALL,
    EIGHTBALL,
    // Idk how to do solides and stripes
    REDS,
    BLUES,
    POTHOLE,
};

int delta;
void update_objects(Collider *colliders)
{
    //TODO: What order should we do this in? 

    int currentTime = RTC_GetTicks();
    delta = currentTime -last_tick;
    last_tick = currentTime;

    float friction = 1.2e-3;
    for (int i = 0; i < OBJECTS; i++)
    {

        if (colliders[i].physics == STATIC || colliders[i].physics == TRIGGER) continue;

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
    // detect_uniform_grid_collision(colliders);   
    detect_basic_collision(colliders, NULL);
    // detect_hgrid_collision(colliders, NULL); 
}

int main(void){
    init_engine();
    Bdisp_EnableColor(1);

    // Create balls
    // FIXME: improve collider creation PLEASE
    Collider colliders[OBJECTS];
    create_colliders(24);

    // pot holes
    float pot_rad = 15;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            int x = pot_rad + i * (WIDTH - pot_rad*2);
            int y = pot_rad + j * (HEIGHT- pot_rad*2);

            Collider pothole = {
                .list = LIST_HEAD_INIT(pothole.list),
                .type = CIRCLE_COLLIDER,
                .physics = TRIGGER,
                .collider.circle = {
                    .radius = pot_rad
                },
                .tag = POTHOLE,
                .x = x, .y = y
            };
            add_collider(pothole);
            colliders[i*2+j] = pothole;
        }
    }

    int radius = 10;
    // cue ball
    Collider cueball = {
        .list = LIST_HEAD_INIT(cueball.list),
        .type = CIRCLE_COLLIDER,
        .physics = DYNAMIC,
        .r = 200, .g = 200, .b = 200,
        .vx = 2, .vy = 0.1,
        .x = 30, .y = HEIGHT/4,
        .collider.circle = {
            .radius = radius
        }
    };
    add_collider(cueball);
    colliders[4] = cueball;
    // place the balls
    int start_y = HEIGHT / 2;
    int start_x = 200;
    float ball_dist = 1;
    for (int level = 1,i=5; i < 20; level++)
    {
        for (int y = level; y < level+level; y++,i++)
        {
            Collider ball = {
                .list = LIST_HEAD_INIT(ball.list),
                .physics = DYNAMIC,
                .type = CIRCLE_COLLIDER,
                .collider.circle.radius = radius,
                .r = 255, .g = 80, .b = 80,
                // .vx = 0, .vy = 0,
                .x = start_x + level*(radius*2 + ball_dist),

                .y = start_y                      // position of the center ball
                    - (radius + ball_dist)   * (level-1)    // go up by by radius * level
                    + (radius*2 + ball_dist) * (y - level)  // go down by the diameter depending on ball no.
            };
            add_collider(ball);
            colliders[i] = ball;
        }
    };

    // place the walls
    const int wall_thickness = 20;
    const int wall_spacing = 35;

    // top walls
    for (int i = 0; i < 2; i++)
    {
        Collider wall = {
            .list = LIST_HEAD_INIT(wall.list),
            .physics =STATIC,
            .type = BOX_COLLIDER,
            .collider.rect = {
                .width = WIDTH-wall_spacing*2,
                .height = wall_thickness,
            },
            .r = 0, .g = 0, .b = 255,
            .x = wall_spacing, .y = i*(HEIGHT-wall_thickness),
        };
        add_collider(wall);
        colliders[i+20] = wall;
    }

    // bottom walls
    for (int i =0; i < 2; i++)
    {
        Collider wall = {
            .list = LIST_HEAD_INIT(wall.list),
            .physics = STATIC,
            .type = BOX_COLLIDER,
            .collider.rect = {
                .width = wall_thickness,
                .height = HEIGHT - wall_spacing*2
            },
            .r = 0, .g = 0, .b = 255,
            .x = i*(WIDTH-wall_thickness), .y = wall_spacing,
        };
        add_collider(wall);
        colliders[22+i] = wall;
    }



    for(;;){
        // Bdisp_AllClr_VRAM();
        Bdisp_Fill_VRAM(rgb565(0,180,0),3);

        // draw black rectangles around the edges
        for (int i =0; i < 2; i++)
        {
            draw_rectangle_filled(0,i*(HEIGHT-wall_thickness),WIDTH,wall_thickness,0);
            draw_rectangle_filled(i*(WIDTH-wall_thickness),0,wall_thickness,HEIGHT,0);
        }

        update_objects(colliders);
        for (int i =0; i < OBJECTS; i++)
        {
            draw_collider(&colliders[i]);
        }
        debug_print("delta: %d", delta);
        if(key_pressed(KEY_PRGM_MENU)){
            int key;
            GetKey(&key);
            break;
        }
        Bdisp_PutDisp_DD();
    }

    return 0;
}
