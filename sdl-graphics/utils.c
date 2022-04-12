#include "utils.h"
#include <sys/param.h>



bool isIntersecting(Collider *first, Collider *second)
{
    Collider *right     = first->x > second->x ? first : second;
    Collider *bottom    = first->y > second->y ? first : second;

    Collider *left      = right == first ? second : first;
    Collider *top       = bottom == first ? second : first;

    // This seems to produce code with less branches but I'm scared to leave it in
    // SDL_Rect *left  = (SDL_Rect *)(-(long int)right + (long int)obj1 + (long int)obj2);
    // SDL_Rect *top   = (SDL_Rect *)(-(long int)bottom + (long int)obj1 + (long int)obj2);

    return 
        (right->x < left->x + left->collider.rect.width) &&
        (bottom->y < top->y + top->collider.rect.height);
}