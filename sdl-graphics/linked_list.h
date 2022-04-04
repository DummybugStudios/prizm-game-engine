#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Heavily inspired by the linux kernel implementation of linked lists

// #define LIST_HEAD_INIT(name) { &(name), &(name) }

typedef struct list_head 
{
    struct list_head *prev;
    struct list_head *next;
} list_head;


// this creates a circular linked list
static inline void list_add(struct list_head *new, struct list_head *head)
{
    list_head *next = head->next;
    next->prev = new;
    new->next = next;
    new->prev = head;
    head->next = new;
}

#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

#endif 