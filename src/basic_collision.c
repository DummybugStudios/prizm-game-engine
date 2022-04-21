#include <engine/basic_collision.h>
#include <engine/constants.h>
#include <engine/utils.h>
#include <stdbool.h> 

void detect_basic_collision(void (* callback)(Collider *, Collider *))
{

    unsigned int size = get_colliders_size();
    for (int i = 0; i < size; i++)
        get_collider(i)->isColliding = false;

    for (int i = 0; i < size; i++)
    {
        Collider *obj1 = get_collider(i); 
        // skip if it's already colliding
        if (obj1->isColliding) continue;

        for (int j = 0; j < size; j++)
        {
            if (i == j)
                continue;
                
            Collider *obj2 = get_collider(j);
            if (isIntersecting(obj1,obj2))
            {
                obj1->isColliding = true;
                obj2->isColliding = true;

                if (obj1->physics == TRIGGER ||obj2->physics == TRIGGER)
                {
                    if (callback)
                        callback(obj1,obj2);
                }
                else
                    handle_collision_physics(obj1,obj2);
                break;
            }
        } // for j

    } // for i
}