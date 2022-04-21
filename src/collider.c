#include <engine/collider.h>
#include <engine/utils.h>

ColliderCollection colliders = {};

void create_colliders(unsigned int size)
{
    // make sure it's not called twice
    assert(colliders.capacity == 0);
    colliders.size = 0; 
    colliders.data = (Collider *)malloc(sizeof(Collider) * size);
    // TODO: Error may not necessarily be fatal.
    if (!colliders.data)
        fatal_error("Could not malloc space for colliders"); 
    colliders.capacity = size;
}

int add_collider(Collider collider)
{
    assert(colliders.size+1 <= colliders.capacity);
    colliders.data[colliders.size] = collider;  
    return colliders.size++;
}

void disable_collider(unsigned int n)
{
    assert(n < colliders.size);
    colliders.data[n].isDisabled = true;
}

void enable_collider(unsigned int n)
{
    assert(n < colliders.size);
    colliders.data[n].isDisabled = false;
}

// If you want to make more specific changes to the collider.
Collider *get_collider(unsigned int n )
{
    assert (n < colliders.size);
    return &colliders.data[n];
}

// Should I provide direct access to the variables? Probably not.

// Get capacity that was set at the start
unsigned int get_colliders_limit()
{
    return colliders.capacity; 
}

// Get the number of objects that are in there right now
unsigned int get_colliders_size()
{ 
    return colliders.size;
}