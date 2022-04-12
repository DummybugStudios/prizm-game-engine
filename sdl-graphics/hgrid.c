#include "hgrid.h"
#include "utils.h"
#include "constants.h"

#include <stdio.h>
#include <sys/param.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

Hgrid hgrid  = {
    .occupiedLevelsMask = 0,
    .tick = 0
};

void init_hgrid()
{
    // initialise linked list heads for objectBucket
    for (int i = 0; i < NUM_BUCKETS; i++)
    {
        hgrid.objectBucket[i].prev = &hgrid.objectBucket[i];
        hgrid.objectBucket[i].next = &hgrid.objectBucket[i];
    }

}

// TODO: look into hash functions more?
int compute_hash_index(int x, int y, int level)
{
    const long int h1 = 0x8da6b343; // Large multiplicative constants;
    const long int h2 = 0xd8163841; // here arbitrarily chosen primes
    const long int h3 = 0xcb1ab31f;

    return (h1 * x + h2 * y + h3 * level) % NUM_BUCKETS; 

}

void add_to_hgrid(Object *object)
{
    int level;
    float size = MIN_CELL_SIZE;
    float diameter = MAX(object->rect.w, object->rect.h);

    for(level = 0; diameter > size; level++)
        size *= CELL_TO_CELL_RATIO;
    
    // This shouldn't happen
    assert(level < HGRID_MAX_LEVELS);

    int bucket = compute_hash_index((object->rect.x/size), (object->rect.y/size), level);
    // printf("bucket-chosen: %d, x: %d, y: %d\n", bucket,object->rect.x, object->rect.y);
    object->level = level;
    object->bucket = bucket;

    //TODO: Do you have to remove it first?
    list_add(&object->list, &hgrid.objectBucket[bucket]);
    hgrid.occupiedLevelsMask |= (1 << level);
    hgrid.objectsAtLevel[level]++;
}

void remove_from_hgrid(Object *object)
{
    if(!--hgrid.objectsAtLevel[object->level])
        hgrid.occupiedLevelsMask &= ~(1 << object->level);

    list_remove(&object->list);
}

void check_hgrid_collision(Object *object)
{
    if (object->isColliding) return;
    float size = MIN_CELL_SIZE;
    int occupiedLevelsMask = hgrid.occupiedLevelsMask; 
//    hgrid.tick++;

    for (int level = 0; level < HGRID_MAX_LEVELS;
    size *= CELL_TO_CELL_RATIO, level++, occupiedLevelsMask >>= 1)
    {
        // check if the rest of the levels are empty
        if (occupiedLevelsMask == 0) break;

        // check if current list is empty 
        if (! (occupiedLevelsMask & 1)) continue;

        float delta = MAX(object->rect.w, object->rect.h);
        int x1 = (int) clamp(floorf((object->posx - delta)/size), 0, WIDTH);
        int y1 = (int) clamp(floorf((object->posy - delta)/size), 0, HEIGHT);

        int x2 = (int) clamp(ceilf((object->posx + delta)/size), 0, WIDTH);
        int y2 = (int) clamp(ceilf((object->posy + delta)/size), 0, HEIGHT);

        for (int x = x1; x <= x2; x++)
        {
            for (int y = y1; y <= y2; y++)
            {
                int bucket = compute_hash_index(x,y,level);
                // TODO: when will this ever happen??
//                if (hgrid.timestamp[bucket] == hgrid.tick) continue;
//                hgrid.timestamp[bucket] = hgrid.tick;

                // printf("x: %d, y: %d, b: %d\n", x, y, bucket);
                list_head *pos; list_for_each(pos, &hgrid.objectBucket[bucket])
                {
                    Object *obj = (Object *)pos; 
                    if (obj == object) continue;
                    // printf("checking %lx against %lx\n", (long int)object & 0xffff, (long int)obj & 0xffff);
                    if (!isIntersecting(object, obj)) continue;
                    object->isColliding = true;
                    obj->isColliding = true;
                    return;
                }
            }
        }
    }
}

void detect_hgrid_collision(Object* object_list)
{
    // FIXME: only re-add it if it has moved.
    // but in this case everything is moving so...?
    for (int i = 0; i < OBJECTS; i++)
    {
        object_list[i].isColliding = false;
        remove_from_hgrid(&object_list[i]);
        add_to_hgrid(&object_list[i]);
    }

    for (int i = 0; i < OBJECTS; i++)
    {
        check_hgrid_collision(&object_list[i]);
    }
}

