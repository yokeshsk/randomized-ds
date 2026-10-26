#ifndef DS_H
#define DS_H
#include <stdio.h>

/* ---------- QuickSort ---------- */
extern long long g_cmp;                       /* comparison counter */
void det_quicksort(int *a, int lo, int hi);   /* pivot = last element  */
void rand_quicksort(int *a, int lo, int hi);  /* pivot = random element */

/* ---------- Skip List ---------- */
#define SL_MAX_LEVEL 16
typedef struct SkipNode {
    int key, height;
    struct SkipNode *forward[SL_MAX_LEVEL];
} SkipNode;
typedef struct { SkipNode *head; int level; int size; } SkipList;
SkipList *sl_create(void);
void sl_insert(SkipList *s, int key);
int  sl_search(SkipList *s, int key);
int  sl_delete(SkipList *s, int key);
void sl_print(SkipList *s);                   /* ASCII levels to stdout */
void sl_dump(SkipList *s, FILE *f);           /* data for plot script   */
void sl_free(SkipList *s);

/* ---------- Treap ---------- */
typedef struct TreapNode {
    int key; unsigned prio;
    struct TreapNode *l, *r;
} TreapNode;
TreapNode *tp_insert(TreapNode *t, int key);
TreapNode *tp_delete(TreapNode *t, int key);
int  tp_search(TreapNode *t, int key);
int  tp_height(TreapNode *t);
void tp_dump(TreapNode *t, FILE *f);          /* "key prio left right" */
void tp_free(TreapNode *t);

/* ---------- Plain (deterministic) BST ---------- */
typedef struct BstNode { int key; struct BstNode *l, *r; } BstNode;
BstNode *bst_insert(BstNode *t, int key);     /* iterative */
int  bst_search(BstNode *t, int key);
int  bst_height(BstNode *t);
void bst_free(BstNode *t);
#endif
