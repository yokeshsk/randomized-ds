#include <stdlib.h>
#include "ds.h"

BstNode *bst_insert(BstNode *t, int key) {
    BstNode *n = malloc(sizeof *n); n->key = key; n->l = n->r = NULL;
    if (!t) return n;
    BstNode *c = t;
    for (;;) {
        if (key == c->key) { free(n); return t; }
        BstNode **next = key < c->key ? &c->l : &c->r;
        if (!*next) { *next = n; return t; }
        c = *next;
    }
}
int bst_search(BstNode *t, int key) {
    while (t) { if (key == t->key) return 1; t = key < t->key ? t->l : t->r; }
    return 0;
}
int bst_height(BstNode *t) {               /* iterative level-order: safe for degenerate trees */
    if (!t) return 0;
    int cap = 1024, head = 0, tail = 0, h = 0;
    BstNode **q = malloc(cap * sizeof *q);
    q[tail++] = t;
    while (head < tail) {
        int lvl = tail - head; h++;
        while (lvl--) {
            BstNode *x = q[head++];
            if (tail + 2 > cap) { cap *= 2; q = realloc(q, cap * sizeof *q); }
            if (x->l) q[tail++] = x->l;
            if (x->r) q[tail++] = x->r;
        }
    }
    free(q); return h;
}
void bst_free(BstNode *t) {                /* iterative */
    while (t) {
        if (t->l) { BstNode *l = t->l; t->l = l->r; l->r = t; t = l; }
        else { BstNode *r = t->r; free(t); t = r; }
    }
}
