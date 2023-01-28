#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct linkedList
{
    int val[2];
    struct linkedList *next;
};

void free_linkedList(struct linkedList *l);
void print_linkedList(struct linkedList *l);
struct linkedList *add_link(int val[2], struct linkedList *first_element);

#endif