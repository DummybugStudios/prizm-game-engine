#include "utils.h"
#include "collider.h"

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