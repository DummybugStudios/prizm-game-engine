#ifndef __SH4A__
// standard library assert for fxcg does not work
#include <assert.h>
#endif 

#include <engine/utils.h>
#include <engine/collider.h>
#include <engine/vector.h>


static inline float square(float a)
{
    return a*a;
}


// Return the squared distance between a point and an AABB
static inline float square_dist_point_rect(Collider *rect, float x, float y)
{
    float sqDist = 0;

    if (x < rect->x) sqDist += square(rect->x - x);
    if (x > rect->x+rect->collider.rect.width) sqDist += square(rect->x + rect->collider.rect.width - x);

    if (y < rect->y) sqDist += square(rect->y - y);
    if (y > rect->y+rect->collider.rect.height) sqDist += square(rect->y + rect->collider.rect.height - y);

    return sqDist;
}


// FIXME: This DOES NOT work with rectangle circle collisions
void handle_collision_physics(Collider *a, Collider*b)
{
    //TODO: make it work with all the colliders
    float m1 = 1;
    float m2 = 1;

    // method credits to https://www.vobarian.com/collisions/2dcollisions2.pdf
    Vector distance = {
            .x = b->x - a->x,
            .y = b->y - a->y,
    };

    Vector normal = normalize(distance);

    Vector tangent = {
            -normal.y,
            normal.x,
    };

    Vector vel_a = {
            .x = a->vx,
            .y = a->vy,
    };

    Vector vel_b = {
            .x = b->vx,
            .y = b->vy,
    };

    // old velocities projected onto the normal vector
    float n_vel_a = dot(vel_a, normal);
    float n_vel_b = dot(vel_b, normal);

    // old velocities projected onto the tangent vector
    // no force in the tangent directions so these don't change
    float t_vel_a = dot(vel_a, tangent);
    float t_vel_b = dot(vel_b, tangent);

    // normal velocities change based on collision
    float n_vel_a_new = (n_vel_a * (m1 - m2) + 2 * m2 * n_vel_b)/(m1 + m2);
    float n_vel_b_new = (n_vel_b * (m2 - m1) + 2 * m1 * n_vel_a)/(m1 + m2);

    // float velocities converted back to our original basis vectors
    Vector n_vel_a_vec = vec_multiply(normal, n_vel_a_new);
    Vector n_vel_b_vec = vec_multiply(normal, n_vel_b_new);

    Vector t_vel_a_vec = vec_multiply(tangent, t_vel_a);
    Vector t_vel_b_vec = vec_multiply(tangent, t_vel_b);

    Vector vel_a_new = vec_add(n_vel_a_vec, t_vel_a_vec);
    Vector vel_b_new = vec_add(n_vel_b_vec, t_vel_b_vec);

    a->vx = vel_a_new.x;
    a->vy = vel_a_new.y;

    b->vx = vel_b_new.x;
    b->vy = vel_b_new.y;

    // solve for time when the objects are separate
    // d = x2 - x1
    // d = t(v2 - v1) + (x2 - x1)
    // t = (d - (x2 - x1)) / (v2 - v1);
    Vector pos_a = {a->x, a->y};
    Vector pos_b = {b->x, b->y};

    float n_pos_a = dot(normal, pos_a);
    float n_pos_b = dot(normal, pos_b);

    float vel_diff = -n_vel_b + n_vel_a;

    float d = a->collider.circle.radius + b->collider.circle.radius;
    if (square(d) - sq_magnitude(distance) < 1 || fabs(vel_diff) < 0.1)
        return;


    float t =  (d - (n_pos_b - n_pos_a)) / vel_diff; 



    a->x += t * (-vel_a.x + vel_a_new.x);
    a->y += t * (-vel_a.y + vel_a_new.y);
    b->x += t * (-vel_b.x + vel_b_new.x);
    b->y += t * (-vel_b.y + vel_b_new.y);
}

bool isIntersecting(Collider *first, Collider *second)
{
    // case 0 vars
    float t;

    // case 1 vars
    Collider *rect = first;
    Collider *circle = second;

    // case 2 vars
    float sqDist;

    switch (first->type + second->type)
    {
        case 0: // both AABBs
            if ((t = first->x - second->x) > second->collider.rect.width || -t > first->collider.rect.width) return false;
            if ((t = first->y - second->y) > second->collider.rect.height || -t > first->collider.rect.height) return false;
            return true;

        case 1: // AABB vs circle
            if (first->type == CIRCLE_COLLIDER)
            {
                circle = first;
                rect = second;
            }
            return square_dist_point_rect(rect, circle->x, circle->y) <= square(circle->collider.circle.radius);

        case 2: // Both circles
            sqDist = square(first->x - second->x) + square(first->y - second->y);
            return sqDist <= square(first->collider.circle.radius + second->collider.circle.radius);
    }
    return 0;
}

// Calculator specific utilities

#ifdef __SH4A__

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <stdarg.h>

#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_NOUNALIGNED
#include "stb_sprintf.h" // for much better sprintf

// function stolen from the libfxcg examples. Don't know why they don't make this a library function.
int key_pressed(int basic_keycode){
    const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
    int row, col, word, bit;
    row = basic_keycode%10;
    col = basic_keycode/10-1;
    word = row>>1;
    bit = col + ((row&1)<<3);
    return (0 != (keyboard_register[word] & 1<<bit));
}

void fatal_error(char *fmt, ...)
{

    char message[255];
    va_list args;
    va_start(args, fmt);
    stbsp_vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    Bdisp_AllClr_VRAM();
    int x = 0;
    int y = 0; 
    PrintMiniMini(&x, &y, message, 0, TEXT_COLOR_BLACK,0);
    for (;;)
    { 
        if (key_pressed(KEY_PRGM_MENU))
        { 
            int key;
            GetKey(&key);
            break;
        }
        Bdisp_PutDisp_DD(); 
    }
}

void debug_print(char *fmt, ...)
{
    char debug[255];
    va_list args;
    va_start(args, fmt);
    stbsp_vsnprintf(debug, sizeof(debug), fmt, args);
    va_end(args);

    int x = 1;
    int y = 20;
    PrintMiniMini(&x, &y, debug, 0, TEXT_COLOR_BLACK, 0);
}

void breakpoint (char *name){
    int key;
    int x = 1;
    int y = LCD_HEIGHT_PX - 50;

    char debug[255];
    stbsp_snprintf(debug, sizeof(debug), "Hit Breakpoint: %s",name);

    PrintMiniMini(&x, &y, debug, 0,TEXT_COLOR_RED,0);
    Bdisp_PutDisp_DD();
    GetKey(&key);
}



#endif 