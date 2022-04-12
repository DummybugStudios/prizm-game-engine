#include "uniform_grid_collision.h"
#include "constants.h"
#include "linked_list.h"
#include "utils.h"

#include <stdbool.h> 

#define GRID_X 14
#define GRID_Y 14

list_head grid[GRID_X][GRID_Y];

void init_uniform_grid()
{
    for (int x = 0; x < GRID_X; x++)
    {
        for (int y = 0; y < GRID_Y; y++)
        {
            grid[x][y].prev = &grid[x][y];
            grid[x][y].next = &grid[x][y];
        }
    }
}


void detect_uniform_grid_collision(Object* objectList)
{
    // fill the grid in based on where objects are
    for (int i = 0; i < OBJECTS; i++)
    {
        int gridX = objectList[i].posx / ((double)(WIDTH + 1) / (GRID_X));
        int gridY = objectList[i].posy / ((double)(HEIGHT +1) / (GRID_Y));
        objectList[i].isColliding = false;

        list_remove(&objectList[i].list);
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
                // don't recheck ones that are already colliding
                if (obj1->isColliding)
                    continue;

                // check against all 8 neighbours
                for (int neighbour_x = -1; neighbour_x < 2; neighbour_x++)
                {
                    if (x + neighbour_x < 0 || x + neighbour_x > GRID_X - 1)
                        continue;
                    for (int neighbour_y = -1; neighbour_y < 2; neighbour_y++)
                    {
                        if (y + neighbour_y < 0 || y + neighbour_y > GRID_Y - 1)
                            continue;

                        list_head *pos2;list_for_each(pos2, &grid[x + neighbour_x][y + neighbour_y])
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
    }
}