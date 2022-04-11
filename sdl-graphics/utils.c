#include "utils.h"
#include <sys/param.h>



bool isIntersecting(Object *first, Object *second)
{
    SDL_Rect *obj1 = &first->rect;
    SDL_Rect *obj2 = &second->rect;

    SDL_Rect *right     = obj1->x > obj2->x ? obj1 : obj2;
    SDL_Rect *bottom    = obj1->y > obj2->y ? obj1 : obj2;

    SDL_Rect *left      = right == obj1 ? obj2 : obj1;
    SDL_Rect *top       = bottom == obj1 ? obj2 : obj1;

    // This seems to produce code with less branches but I'm scared to leave it in
    // SDL_Rect *left  = (SDL_Rect *)(-(long int)right + (long int)obj1 + (long int)obj2);
    // SDL_Rect *top   = (SDL_Rect *)(-(long int)bottom + (long int)obj1 + (long int)obj2);

    return 
        (right->x < left->x + left->w) &&
        (bottom->y < top->y + top->h);
}