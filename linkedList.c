#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

void free_linkedList(struct linkedList *l)
{
    struct linkedList *current;
    current = l;
    while (current != NULL) // marqueur de fin de liste = pointeur NULL
    {
        struct linkedList *next;
        next = current->next;
        free(current);
        current = next;
    }
}

void print_linkedList(struct linkedList *l)
{
    struct linkedList *current;
    current = l;
    while (current != NULL)
    {
        printf("(%d, %d), ", current->val[0], current->val[1]);
        current = current->next;
    }
    printf("\n");
}

// crée un maillon de liste chainée
// et le relie au début la liste pointée par first_element
struct linkedList *add_link(int val[2], struct linkedList *first_element)
{
    struct linkedList *current;
    current = malloc(sizeof(struct linkedList));
    current->val[0] = val[0];
    current->val[1] = val[1];
    current->next = first_element;
    return current;
}