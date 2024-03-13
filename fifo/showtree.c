#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "showtree.h"
struct Tree* init(void)
{
    struct Tree *tree = malloc(sizeof(struct Tree));
    tree->num = 0;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}
static bool insertNode(struct Tree **tree, int num)
{
    if (*tree == NULL)
    {
        *tree = malloc(sizeof(struct Tree));
        if (*tree == NULL)
            return false;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->num = num;
        return true;
    }
    if ((*tree)->num > num)
        return insertNode(&(*tree)->left, num);
    return insertNode(&(*tree)->right, num);
}
bool insert(struct Tree **tree, int num)
{
    if (*tree == NULL)
        *tree = init();
    if ((*tree)->num == 0)
    {
        (*tree)->num = num;
        return true;
    }
    return insertNode(tree, num);
}
static bool deleteNode(struct Tree **tree)
{
    if (*tree == NULL)
        return false;
    struct Tree *p;
    if ((*tree)->left == NULL)
    {
        p = *tree;
        *tree = (*tree)->right;
        free(p);
        p = NULL;
    }
    else if ((*tree)->right == NULL)
    {
        p = *tree;
        *tree = (*tree)->left;
        free(p);
        p = NULL;
    }
    else
    {
        p = *tree;
        struct Tree *s = (*tree)->left;
        while (s->right != NULL)
        {
            p = s;
            s = s->right;
        }
        (*tree)->num = s->num;
        if (*tree == p)
        {
            (*tree)->left = s->left;
            free(s);
            s = NULL;
        }
        else
        {
            p->right = s->left;
            free(s);
            s = NULL;
        }
    }
    return true;
}
bool delete(struct Tree **tree, int num)
{
    if (*tree == NULL)
        return false;
    if ((*tree)->num == num)
        return deleteNode(tree);
    if ((*tree)->num > num)
        return delete(&(*tree)->left, num);
    return delete(&(*tree)->right, num);
}
void print(const struct Tree *tree)
{
    putchar('(');
    if (tree == NULL)
    {
        putchar(')');
        return;
    }
    printf("%d", tree->num);
    print(tree->left);
    print(tree->right);
    putchar(')');
    fflush(stdout);
    return;
}
void destroy(struct Tree **tree)
{
    if (*tree == NULL)
        return;
    destroy(&(*tree)->left);
    destroy(&(*tree)->right);
    free(*tree);
    *tree = NULL;
    return;
}
