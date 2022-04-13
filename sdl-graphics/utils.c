#include <assert.h>
#include "utils.h"
#include "collider.h"
#include "vector.h"

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

void handle_collision_physics(Collider *a, Collider*b)
{
    //TODO: make it work with all the colliders
    float m1 = 1;
    float m2 = 1;

    // method credits to https://www.vobarian.com/collisions/2dcollisions2.pdf
    Vector normal = {
            .x = b->x - a->x,
            .y = b->y - a->y,
    };
    normal = normalize(normal);

    Vector tangent = {
            -normal.y,
            normal.x,
    };

    Vector vel_a = {
            .x = a->vx,
            .y = a->vy,
    };

    Vector vel_b = {
            .x = b->vx,
            .y = b->vy,
    };

    // old velocities projected onto the normal vector
    float n_vel_a = dot(vel_a, normal);
    float n_vel_b = dot(vel_b, normal);

    // old velocities projected onto the tangent vector
    // no force in the tangent directions so these don't change
    float t_vel_a = dot(vel_a, tangent);
    float t_vel_b = dot(vel_b, tangent);

    // normal velocities change based on collision
    float n_vel_a_new = (n_vel_a * (m1 - m2) + 2 * m2 * n_vel_b)/(m1 + m2);
    float n_vel_b_new = (n_vel_b * (m2 - m1) + 2 * m1 * n_vel_a)/(m1 + m2);

    // float velocities converted back to our original basis vectors
    Vector n_vel_a_vec = vec_multiply(normal, n_vel_a_new);
    Vector n_vel_b_vec = vec_multiply(normal, n_vel_b_new);

    Vector t_vel_a_vec = vec_multiply(tangent, t_vel_a);
    Vector t_vel_b_vec = vec_multiply(tangent, t_vel_b);

    Vector vel_a_new = vec_add(n_vel_a_vec, t_vel_a_vec);
    Vector vel_b_new = vec_add(n_vel_b_vec, t_vel_b_vec);

    a->vx = vel_a_new.x;
    a->vy = vel_a_new.y;

    b->vx = vel_b_new.x;
    b->vy = vel_b_new.y;
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