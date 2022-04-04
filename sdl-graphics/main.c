#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <sys/param.h>
#include <stdlib.h>
#include <time.h> // for benchmarking
#include <string.h> // for memset

#include "linked_list.h"

#define OBJECTS 100
#define WIDTH 640
#define HEIGHT 480  
#define SAMPLE_SIZE 200

#define GRID_X 4
#define GRID_Y 4

bool enableBreakpoints = false;
SDL_Window *window;

typedef struct Object
{
    list_head list;

    SDL_Rect rect;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    float vx;
    float vy;
    float posx;
    float posy;
    bool isColliding;

} Object;
 
 
// GRID as an array of doubly linked list of objects
list_head grid[GRID_X][GRID_Y];

void drawObject(SDL_Renderer* renderer, Object *object)
{
    if (object->isColliding)
        SDL_SetRenderDrawColor(renderer, 255,255,0,255);
    else
        SDL_SetRenderDrawColor(renderer, object->r,object->g,object->b,255);
    SDL_RenderFillRect(renderer, &object->rect);
}

bool isIntersecting(Object *first, Object *second)
{
    SDL_Rect *obj1 = &first->rect;
    SDL_Rect *obj2 = &second->rect;

    return 
        MAX(obj1->x, obj2->x) - MIN(obj1->x, obj2->x) < obj1->w && 
        MAX(obj1->y, obj2->y) - MIN(obj1->y, obj2->y) < obj2->h;

}

// Uniform Grid collision Detection
void detectUniformGridCollision(Object* objectList)
{
    // reset the grid
    // TODO: don't reset it every time please
    for (int x = 0; x < GRID_X; x++)
    {
        for (int y = 0; y < GRID_Y; y++)
        {
            // TODO: automate this please
            grid[x][y].prev = &grid[x][y];
            grid[x][y].next = &grid[x][y];
        }
    }

    // fill the grid in based on where objects are
    // TODO: this is a terrible way to do this
    for (int i = 0; i < OBJECTS; i++)
    {
        // ensure that the object is not part of any other lists first
        objectList[i].list.next = &objectList[i].list;
        objectList[i].list.prev = &objectList[i].list;

        int gridX = objectList[i].posx / ((double)(WIDTH + 1) / (GRID_X));
        int gridY = objectList[i].posy / ((double)(HEIGHT +1) / (GRID_Y));
    
        list_add(&objectList[i].list,&grid[gridX][gridY]);
    }

    // for all grids check if the objects in them are intersecting
    for (int x = 0; x < GRID_X; x++)
    {
        for (int y = 0; y < GRID_Y; y++)
        {
            // check if it's empty
            if (grid[x][y].next == &grid[x][y])
                continue;

            //TODO: better way to loop over the linked list? 
            list_head *pos1; list_for_each(pos1, &grid[x][y])
            {
                Object *obj1 = (Object *)pos1;
                obj1->isColliding = false;

                list_head *pos2; list_for_each(pos2, &grid[x][y])
                {
                    if (pos1 == pos2)
                        continue;
                    
                    Object *obj2 = (Object *)pos2;

                    if (isIntersecting (obj1, obj2))
                    {
                        obj1->isColliding = true;
                        obj2->isColliding = true;
                        break;
                    }
                }

            }
        }
    }
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
    detectUniformGridCollision(objectList); 
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

        objects[i].vx = (rand() % 2 == 0 ? 1 : -1) * (rand() % 40 + 1)/10.0f;
        objects[i].vy = (rand() % 2 == 0 ? 1 : -1) * (rand() % 40 + 1)/10.0f;
        objects[i].posx = objects[i].rect.x = rand() % WIDTH;
        objects[i].posy = objects[i].rect.y = rand() % HEIGHT;
        objects[i].rect.h = objects[i].rect.w = 20;

        objects[i].isColliding = false;
    }
    
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