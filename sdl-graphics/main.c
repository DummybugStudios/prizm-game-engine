#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h> // for benchmarking

#include "constants.h"
#include "object.h"
#include "utils.h" // TODO: maybe not needed
#include "uniform_grid_collision.h"

#define SAMPLE_SIZE 200

#define GRID_X 16
#define GRID_Y 16

bool enableBreakpoints = false;
SDL_Window *window;


// Quickly render square
void drawObject(SDL_Renderer* renderer, Object *object)
{
    if (object->isColliding)
        SDL_SetRenderDrawColor(renderer, 255,255,0,255);
    else
        SDL_SetRenderDrawColor(renderer, object->r,object->g,object->b,255);
    SDL_RenderFillRect(renderer, &object->rect);
}

// BASIC collision detection
void detectCollisions(Object *objectList)
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
void updateObjects(Object *objectList)
{
    for (int i = 0; i < OBJECTS; i++)
    {
        objectList[i].posx += objectList[i].vx;
        objectList[i].posy += objectList[i].vy;

        if (objectList[i].posy < 0 || objectList[i].posy > HEIGHT)
        {
            // very cool branchless way to clamp values
            objectList[i].posy = (objectList[i].posy > 0) * HEIGHT;
            objectList[i].vy *= -1; 
        }

        if (objectList[i].posx < 0 || objectList[i].posx > WIDTH)
        {
            objectList[i].posx = (objectList[i].posx > 0) * WIDTH;
            objectList[i].vx *= -1;
        }   
        objectList[i].rect.x = objectList[i].posx;
        objectList[i].rect.y = objectList[i].posy;
    }
    // detectCollisions(objectList);
    detect_uniform_grid_collision(objectList); 
}


int main(int argc, char **argv)
{

    srand(0);
    // Initialise SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "Could not initialise SDL\n");
        exit(-1);
    }

    // Set up rendering
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    window = SDL_CreateWindow("Collisions", 0,50,640,480,rendererFlags);
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

    // make 5 objects
    Object *objects = (Object *)malloc(OBJECTS * sizeof(Object));
    for (int i = 0; i < OBJECTS; i++)
    {
        // initially all lists are empty.
        objects[i].list.next = &objects[i].list;
        objects[i].list.prev = &objects[i].list;

        objects[i].r = 255;
        objects[i].g = objects[i].b = 0;

        objects[i].vx = (rand() % 2 == 0 ? 1 : -1) * (rand() % 40 + 1) / 10.0f;
        objects[i].vy = (rand() % 2 == 0 ? 1 : -1) * (rand() % 40 + 1) / 10.0f;
        objects[i].posx = objects[i].rect.x = rand() % WIDTH;
        objects[i].posy = objects[i].rect.y = rand() % HEIGHT;
        objects[i].rect.h = objects[i].rect.w = 20;

        objects[i].isColliding = false;
    }

    init_uniform_grid();
    
    clock_t start, end;
    double avg_time = 0;
    // infinite loop but keep track of iteration for time used
    for (int j = 0; j >= 0;j++)
    {
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderClear(renderer);
        
        start = clock(); 
        updateObjects(objects);
        end = clock();
    
        // only store when not in a debugger
        j--;
        if (j < SAMPLE_SIZE && !enableBreakpoints)
        {
            avg_time += ((double)(end - start)) / CLOCKS_PER_SEC;
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
                printf("Average time to update: %f\n", avg_time/SAMPLE_SIZE);
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