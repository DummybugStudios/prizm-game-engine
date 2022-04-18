#include <engine/basic_collision.h>
#include <engine/constants.h>
#include <engine/utils.h>
#include <stdbool.h> 

void detect_basic_collision(Collider *objectList)
{
    for (int i = 0; i < OBJECTS; i++)
        objectList[i].isColliding = false;

    for (int i = 0; i < OBJECTS; i++)
    {
        // skip if it's already colliding
        if (objectList[i].isColliding) continue;

        for (int j = 0; j < OBJECTS; j++)
        {
            if (i == j)
                continue;

            if (isIntersecting(&objectList[i], &objectList[j]))
            {
                objectList[i].isColliding = true;
                objectList[j].isColliding = true;
                handle_collision_physics(&objectList[i], &objectList[j]);
                break;
            }
        } // for j

    } // for i
}