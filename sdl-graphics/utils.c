#include "utils.h"
#include <sys/param.h>


bool isIntersecting(Object *first, Object *second)
{
    SDL_Rect *obj1 = &first->rect;
    SDL_Rect *obj2 = &second->rect;

    return 
        MAX(obj1->x, obj2->x) - MIN(obj1->x, obj2->x) < obj1->w && 
        MAX(obj1->y, obj2->y) - MIN(obj1->y, obj2->y) < obj2->h;

}