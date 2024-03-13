#ifndef SHOWTREE_H
#define SHOWTREE_H
#include <stdbool.h>
struct Tree
{
    int num;
    struct Tree *left;
    struct Tree *right;
};
extern struct Tree* init(void);
extern bool insert(struct Tree**, int);
extern bool delete(struct Tree**, int);
extern void print(const struct Tree*);
extern void destroy(struct Tree**);
#endif
