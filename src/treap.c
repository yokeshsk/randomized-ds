#include <stdlib.h>
#include "ds.h"

static TreapNode *new_node(int key) {
    TreapNode *n = malloc(sizeof *n);
    n->key = key;
    n->prio = ((unsigned)rand() << 16) ^ (unsigned)rand();   /* random priority */
    n->l = n->r = NULL;
    return n;
}
static TreapNode *rot_right(TreapNode *y) { TreapNode *x = y->l; y->l = x->r; x->r = y; return x; }
static TreapNode *rot_left (TreapNode *x) { TreapNode *y = x->r; x->r = y->l; y->l = x; return y; }

/* BST order on key, MAX-heap order on priority */
TreapNode *tp_insert(TreapNode *t, int key) {
    if (!t) return new_node(key);
    if (key == t->key) return t;
    if (key < t->key) { t->l = tp_insert(t->l, key); if (t->l->prio > t->prio) t = rot_right(t); }
    else              { t->r = tp_insert(t->r, key); if (t->r->prio > t->prio) t = rot_left(t); }
    return t;
}
TreapNode *tp_delete(TreapNode *t, int key) {
    if (!t) return NULL;
    if (key < t->key) t->l = tp_delete(t->l, key);
    else if (key > t->key) t->r = tp_delete(t->r, key);
    else {
        if (!t->l || !t->r) { TreapNode *c = t->l ? t->l : t->r; free(t); return c; }
        if (t->l->prio > t->r->prio) { t = rot_right(t); t->r = tp_delete(t->r, key); }
        else                         { t = rot_left(t);  t->l = tp_delete(t->l, key); }
    }
    return t;
}
int tp_search(TreapNode *t, int key) {
    while (t) { if (key == t->key) return 1; t = key < t->key ? t->l : t->r; }
    return 0;
}
int tp_height(TreapNode *t) {
    if (!t) return 0;
    int a = tp_height(t->l), b = tp_height(t->r);
    return 1 + (a > b ? a : b);
}
void tp_dump(TreapNode *t, FILE *f) {      /* pre-order: key prio leftkey rightkey (-1 = none) */
    if (!t) return;
    fprintf(f, "%d %u %d %d\n", t->key, t->prio, t->l ? t->l->key : -1, t->r ? t->r->key : -1);
    tp_dump(t->l, f); tp_dump(t->r, f);
}
void tp_free(TreapNode *t) { if (!t) return; tp_free(t->l); tp_free(t->r); free(t); }
