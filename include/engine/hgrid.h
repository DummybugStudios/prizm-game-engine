#ifndef HGRID_H
#define HGRID_H 
#include <engine/collider.h>

#define HGRID_MAX_LEVELS 4
#define NUM_BUCKETS 200 // Todo: how many should it have? 

#define MIN_CELL_SIZE 15.0f
#define CELL_TO_CELL_RATIO 2.0f

typedef struct Hgrid {
    int occupiedLevelsMask;
    int objectsAtLevel[HGRID_MAX_LEVELS];
    list_head objectBucket[NUM_BUCKETS];
    int timestamp[NUM_BUCKETS];
    int tick;
} Hgrid;

void init_hgrid();

void add_to_hgrid(Collider *object);

void detect_hgrid_collision(Collider *object_list);

#endif