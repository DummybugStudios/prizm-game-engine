#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h> // for benchmarking

#include <engine/constants.h>
#include <engine/collider.h>
#include <engine/utils.h>

#include <engine/basic_collision.h> 
#include <engine/uniform_grid_collision.h>
#include <engine/hgrid.h>

#include <SDL2_gfxPrimitives.h>

#define SAMPLE_SIZE 200

SDL_Window *window;

// Quickly render square
void drawObject(SDL_Renderer* renderer, Collider *collider)
{
    SDL_Rect rect = {
            .x = collider->x,
            .y = collider->y,
            .h = collider->collider.rect.height,
            .w = collider->collider.rect.width,
    };
    switch (collider->type)
    {
        case (BOX_COLLIDER):
            if (collider->isColliding)
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, collider->r, collider->g, collider->b, 255);
            SDL_RenderFillRect(renderer, &rect);
            break;
        case (CIRCLE_COLLIDER):
            if (collider->isColliding)
                filledCircleRGBA(renderer,collider->x, collider->y, collider->collider.circle.radius, 255,255,0, 0xff);
            else
                filledCircleRGBA(renderer,collider->x, collider->y, collider->collider.circle.radius, collider->r, collider->g, collider->b, 0xff);
            break;
    };
}

// Basic Physics Update
void updateObjects(Collider *objectList)
{
    for (int i = 0; i < OBJECTS; i++)
    {
        objectList[i].x += objectList[i].vx;
        objectList[i].y += objectList[i].vy;

        if (objectList[i].y < 0 || objectList[i].y > HEIGHT)
        {
            // very cool branchless way to clamp values
            objectList[i].y = (objectList[i].y > 0) * HEIGHT;
            objectList[i].vy *= -1; 
        }

        if (objectList[i].x < 0 || objectList[i].x > WIDTH)
        {
            objectList[i].x = (objectList[i].x > 0) * WIDTH;
            objectList[i].vx *= -1;
        }   
    }
    detect_basic_collision(objectList);
    //  detect_uniform_grid_collision(objectList);
//    detect_hgrid_collision(objectList);
}


int main(int argc, char **argv)
{
    enableBreakpoints = false; 
    srand(2);
    // Initialise SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "Could not initialise SDL\n");
        exit(-1);
    }

    // Set up rendering
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    window = SDL_CreateWindow("Collisions", 0,0,WIDTH,HEIGHT,rendererFlags);
    if (!window)
    {
        fprintf(stderr, "Could not get window\n");
        exit(-1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, rendererFlags);
    if (!renderer)
    {
        fprintf(stderr, "Could not get renderer\n");
        exit(-1); 
    }

    Collider objects[OBJECTS];
    // make box
    Collider box = {
        .list = LIST_HEAD_INIT(box.list),
        .type = BOX_COLLIDER,
        .physics = STATIC,
        .r = 255, .g = 0, .b = 0,
        .vx = 0, .vy =0,
        .x = 10, .y = 10,
        .collider = {
            .rect = {
                .width = 80,
                .height = 600,
            }
        }
    };

    Collider box2 = {
        .list = LIST_HEAD_INIT(box.list),
        .type = BOX_COLLIDER,
        .physics = STATIC,
        .r = 255, .g = 0, .b = 0,
        .vx = 0, .vy =0,
        .x = 400, .y = 10,
        .collider = {
            .rect = {
                .width = 80,
                .height = 600,
            }
        }
    };

    Collider circle = {
        .list = LIST_HEAD_INIT(circle.list),
        .type = CIRCLE_COLLIDER,
        .physics = DYNAMIC,
        .r = 0, .g = 255, .b =0,
        .vx = -3, .vy = 1,
        .x = 200, .y = 10,
        .collider = {
            .circle = {
                .radius = 20
            }
        }
    };
    objects[0] = box;
    objects[1] = circle;
    objects[2] = box2;

    init_uniform_grid();
    
    clock_t start, end;
    double avg_time = 0;
    // infinite loop but keep track of iteration for time used
    int speed = 300;
    for (int j = 0; j >= 0;)
    {
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderClear(renderer);
        
        start = clock(); 
        updateObjects(objects);
        end = clock();
    
        // only store when not in a debugger
        if (j < SAMPLE_SIZE && !enableBreakpoints)
        {
            avg_time += ((double)(end - start)) / CLOCKS_PER_SEC;
            j++;
        }
        else if (j == SAMPLE_SIZE)
        {
            printf("Average time to update: %f\n", avg_time/SAMPLE_SIZE);
            j++;
        }

        // draw all objects
        for (int i = 0; i < OBJECTS; i++)
        {
            drawObject(renderer, &objects[i]);
        }

        SDL_Event event;

        while (SDL_PollEvent(&event))
        { 
            if (event.type == SDL_QUIT)
            {
                return 0;
            }
            // TODO: can it be rewritten with a switch? 
            else if (event.type == SDL_KEYDOWN)
            {
                // for debugging purposes
                if (event.key.keysym.scancode == SDL_SCANCODE_E)
                {
                    enableBreakpoints = !enableBreakpoints;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    speed = speed == 16 ? 300 : 16;  
                }
            }

        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(speed);
    }

    return 0;

}