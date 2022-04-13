#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h> // for benchmarking

#include "constants.h"
#include "collider.h"
#include "utils.h" // TODO: maybe not needed
#include "uniform_grid_collision.h"
#include "hgrid.h"
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

// BASIC collision detection
void detectCollisions(Collider *objectList)
{
    bool collisions[OBJECTS];
    memset(collisions, false, sizeof(collisions)); 
    for (int i = 0; i < OBJECTS; i++)
    {
        // skip if it's already colliding
        if (collisions[i])
            continue;

        bool found = false;
        for (int j = 0; j < OBJECTS; j++)
        {
            if (i == j)
                continue;

            if (isIntersecting(&objectList[i], &objectList[j]))
            {
                collisions[i] = collisions[j] = true;
                objectList[i].isColliding = true;
                objectList[j].isColliding = true;

                // find the new velocities

                found = true;
                break;
            }
        } // for j

        if (!found)
            objectList[i].isColliding = false;

    } // for i
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
//     detectCollisions(objectList);
     detect_uniform_grid_collision(objectList);
//    detect_hgrid_collision(objectList);
}


int main(int argc, char **argv)
{
    enableBreakpoints = false; 
    srand(0);
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

    init_hgrid();
    // make 5 objects
    Collider *objects = (Collider *)malloc(OBJECTS * sizeof(Collider));
    for (int i = 0; i < OBJECTS; i++)
    {
        // initially all lists are empty.
        objects[i].list.next = &objects[i].list;
        objects[i].list.prev = &objects[i].list;

        if (i % 2) {
            objects[i].type = CIRCLE_COLLIDER;
            objects[i].collider.circle.radius = 20;
        }
        else {
            objects[i].type = BOX_COLLIDER;
            objects[i].collider.rect.height = objects[i].collider.rect.width = 40;
        }

        objects[i].r = 255;
        objects[i].g = objects[i].b = 0;

        objects[i].vx = (rand() % 2 == 0 ? 1 : -1) * (rand() % 40 + 1) / 10.0f;
        objects[i].vy = (rand() % 2 == 0 ? 1 : -1) * (rand() % 40 + 1) / 10.0f;
        objects[i].x = rand() % WIDTH;
        objects[i].y = rand() % HEIGHT;

        objects[i].isColliding = false;
        add_to_hgrid(&objects[i]);

    }

    init_uniform_grid();
    
    clock_t start, end;
    double avg_time = 0;
    // infinite loop but keep track of iteration for time used
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
                exit(-1);
            }
            // TODO: can it be rewritten with a switch? 
            else if (event.type == SDL_KEYDOWN)
            {
                // for debugging purposes
                if (event.key.keysym.scancode == SDL_SCANCODE_E)
                {
                    enableBreakpoints = !enableBreakpoints;
                }
            }

        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return 0;

}