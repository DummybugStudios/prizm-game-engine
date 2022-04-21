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
void update_objects()
{
    //TODO: What order should we do this in? 

    int currentTime = RTC_GetTicks();
    delta = currentTime -last_tick;
    last_tick = currentTime;

    float friction = 1.2e-3;
    for (int i = 0; i < get_colliders_size(); i++)
    {
        Collider *object = get_collider(i);
        if (object->physics == STATIC || object->physics == TRIGGER) continue;

        object->x += delta * object->vx;
        object->y += delta * object->vy; 

        object->vx -= delta*friction*object->vx;
        object->vy -= delta*friction*object->vy;

        if (fabs(object->vx) < 1e-2) object->vx = 0;
        if (fabs(object->vy) < 1e-2) object->vx =0;
        

        int radius = object->collider.circle.radius;

        if (object->x < radius || object->x > WIDTH - radius)
        {
            object->x = object->x < radius ? radius : WIDTH - radius-1;
            object->vx *= -1;
        }
        if (object->y < radius || object->y > HEIGHT - radius)
        {
            object->y = object->y < radius ? radius : HEIGHT - radius-1;
            object->vy *= -1;
        }
    }
    // detect_uniform_grid_collision(colliders);   
    detect_basic_collision(NULL);
    // detect_hgrid_collision(colliders, NULL); 
}

int main(void){
    init_engine();
    Bdisp_EnableColor(1);

    // Create balls
    // FIXME: improve collider creation PLEASE
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
        }
    }

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

        update_objects();
        for (int i =0; i < get_colliders_size(); i++)
        {
            draw_collider(get_collider(i));
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
