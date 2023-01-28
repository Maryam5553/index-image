#ifndef NODE_H
#define NODE_H
#include "linkedList.h"

struct node
{
    unsigned char col[3];
    struct linkedList *index; // tous les endroits où cette couleur apparaît dans l'image
    struct node *left;
    struct node *right;
    int height;
};

struct node *new_node(unsigned char col[3], int first_pos[2]);
void free_node(struct node *p);
void nb_colors(struct node *p, int *n);
void print_node(struct node *p);
void add_pos(int pos[2], struct node *p);

// rééquilibrage de l'arbre AVL
int max(int a, int b);
int height(struct node *p);
int get_balance(struct node *p);
struct node *right_rotate(struct node *y);
struct node *left_rotate(struct node *x);

// fonctions utiles pour la palette
struct node *insert(struct node *p, unsigned char x[3], int pos[2]);
unsigned char *create_palette(struct node *p, unsigned char *pal, int *n);
void index_image(struct node *p, struct image *image, struct pal_image *new_image);

#endif