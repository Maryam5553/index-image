#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pngio.h"
#include "node.h"

// crée un arbre avec pour racine la couleur donnée "col"
// initialise l'index à la première position où elle apparait
struct node *new_node(unsigned char col[3], int first_pos[2])
{
    struct node *p;
    p = malloc(sizeof(struct node));
    p->col[0] = col[0];
    p->col[1] = col[1];
    p->col[2] = col[2];

    struct linkedList *l;
    l = malloc(sizeof(struct linkedList));
    l->val[0] = first_pos[0];
    l->val[1] = first_pos[1];
    l->next = NULL;
    p->index = l;

    p->left = NULL;
    p->right = NULL;
    p->height = 1;
    return p;
}

void free_node(struct node *p)
{
    if (p == NULL)
        return;
    free_node(p->left);
    free_node(p->right);
    free_linkedList(p->index);
    free(p);
}

void nb_colors(struct node *p, int *n)
{
    if (p == NULL)
        return;
    nb_colors(p->left, n);
    (*n)++;
    nb_colors(p->right, n);
}

void print_node(struct node *p)
{
    if (p == NULL)
        return;
    print_node(p->left);
    printf("col: %d, %d, %d\n", p->col[0], p->col[1], p->col[2]);
    printf("pos: ");
    print_linkedList(p->index);
    print_node(p->right);
}

// ajoute une nouvelle position (où la couleur apparait) dans l'index
void add_pos(int pos[2], struct node *p)
{
    p->index = add_link(pos, p->index);
}

int max(int a, int b)
{
    if (a >= b)
        return a;
    return b;
}

int height(struct node *p)
{
    if (p == NULL)
        return 0;
    return p->height;
}

int get_balance(struct node *p)
{
    if (p == NULL)
        return 0;
    return height(p->left) - height(p->right);
}

struct node *right_rotate(struct node *y)
{
    struct node *x = y->left;

    y->left = x->right;
    x->right = y;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

struct node *left_rotate(struct node *x)
{
    struct node *y = x->right;

    x->right = y->left;
    y->left = x;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// pour comparer R G et B
bool insert_from_col(struct node *p, unsigned char col[3], int pos[2], int i)
{
    if (col[i] != p->col[i])
    {
        if (col[i] < p->col[i])
            p->left = insert(p->left, col, pos);
        else
            p->right = insert(p->right, col, pos);
        return true;
    }
    else
        return false;
}

// ajoute une couleur dans l'arbre
// si elle n'est pas déjà là, rajoute sa position
struct node *insert(struct node *p, unsigned char col[3], int pos[2])
{
    if (p == NULL)
        return new_node(col, pos);

    if (!insert_from_col(p, col, pos, 0))
        if (!insert_from_col(p, col, pos, 1))
            if (!insert_from_col(p, col, pos, 2))
                add_pos(pos, p);

    // rééquilibrage de l'arbre
    /* p->height = 1 + max (height(p->left) , height(p->right)); */

    /* int balance = get_balance(p); */
    /* if (balance > 1) { */
    /* 	if (x[0] != p->left->col[0]) { */
    /* 	    if (x[0] < p->left->col[0]) */
    /* 		return right_rotate (p); */
    /* 	    else { */
    /* 		p->left = left_rotate (p->left); */
    /* 		return right_rotate (p); */
    /* 	     } */
    /* 	} */
    /* 	else if (x[1] != p->left->col[1]) { */
    /* 	    if (x[1] < p->left->col[1]) */
    /* 		return right_rotate (p); */
    /* 	    else { */
    /* 		p->left = left_rotate (p->left); */
    /* 		return right_rotate (p); */
    /* 	     } */
    /* 	} */
    /* 	else if (x[2] != p->left->col[2]) { */
    /* 	    if (x[2] < p->left->col[2]) */
    /* 		return right_rotate (p); */
    /* 	    else { */
    /* 		p->left = left_rotate (p->left); */
    /* 		return right_rotate (p); */
    /* 	     } */
    /* 	} */
    /* } */

    /* if (balance < -1) { */
    /* 	if (x[0] != p->left->col[0]) { */
    /* 	    if (x[0] < p->left->col[0]) */
    /* 		return left_rotate (p); */
    /* 	    else { */
    /* 		p->right = right_rotate (p->right); */
    /* 		return left_rotate (p); */
    /* 	     } */
    /* 	} */
    /* 	else if (x[1] != p->left->col[1]) { */
    /* 	    if (x[1] < p->left->col[1]) */
    /* 		return left_rotate (p); */
    /* 	    else { */
    /* 		p->right = right_rotate (p->right); */
    /* 		return left_rotate (p); */
    /* 	     } */
    /* 	} */
    /* 	else if (x[2] != p->left->col[2]) { */
    /* 	    if (x[2] < p->left->col[2]) */
    /* 		return left_rotate (p); */
    /* 	    else { */
    /* 		p->right = right_rotate (p->right); */
    /* 		return left_rotate (p); */
    /* 	     } */
    /* 	} */
    /* } */

    return p;
}

unsigned char *create_palette(struct node *p, unsigned char *pal, int *n)
{
    if (p == NULL)
        return pal;
    pal[(*n) * 3] = p->col[0];
    pal[(*n) * 3 + 1] = p->col[1];
    pal[(*n) * 3 + 2] = p->col[2];
    (*n)++;
    pal = create_palette(p->left, pal, n);
    pal = create_palette(p->right, pal, n);
    return pal;
}

int dist(unsigned char *image_col, unsigned char *pal_col, int k)
{
    return (image_col[0] - pal_col[k * 3]) * (image_col[0] - pal_col[k * 3]) +
           (image_col[1] - pal_col[k * 3 + 1]) * (image_col[1] - pal_col[k * 3 + 1]) +
           (image_col[2] - pal_col[k * 3 + 2]) * (image_col[2] - pal_col[k * 3 + 2]);
}

void index_image(struct node *p, struct image *image, struct pal_image *new_image)
{
    if (p == NULL)
        return;

    int diff_min = 3 * 256 * 256;
    int new_color;
    int diff;

    // cherche la couleur de la palette la plus proche
    for (int k = 0; k < new_image->pal_len; k++)
    {
        diff = dist(p->col, new_image->pal, k);
        if (diff < diff_min)
        {
            diff_min = diff;
            new_color = k;
        }
    }

    // remplace toutes les instances de la couleur, par la couleur de la palette la plus proche
    struct linkedList *index;
    index = p->index;
    while (index != NULL)
    {
        new_image->data[index->val[0]][index->val[1]] = new_color;
        index = index->next;
    }

    index_image(p->left, image, new_image);
    index_image(p->right, image, new_image);
    return;
}