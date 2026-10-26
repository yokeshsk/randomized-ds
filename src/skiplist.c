#include <stdlib.h>
#include "ds.h"

static SkipNode *new_node(int key) {
    SkipNode *n = calloc(1, sizeof *n);
    n->key = key;
    return n;
}
SkipList *sl_create(void) {
    SkipList *s = malloc(sizeof *s);
    s->head = new_node(0);
    s->level = 1; s->size = 0;
    return s;
}
static int random_level(void) {            /* coin flips, p = 1/2 */
    int lvl = 1;
    while (lvl < SL_MAX_LEVEL && (rand() & 1)) lvl++;
    return lvl;
}
int sl_search(SkipList *s, int key) {
    SkipNode *x = s->head;
    for (int i = s->level - 1; i >= 0; i--)
        while (x->forward[i] && x->forward[i]->key < key) x = x->forward[i];
    x = x->forward[0];
    return x && x->key == key;
}
void sl_insert(SkipList *s, int key) {
    SkipNode *update[SL_MAX_LEVEL], *x = s->head;
    for (int i = s->level - 1; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->key < key) x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];
    if (x && x->key == key) return;        /* no duplicates */
    int lvl = random_level();
    if (lvl > s->level) {
        for (int i = s->level; i < lvl; i++) update[i] = s->head;
        s->level = lvl;
    }
    SkipNode *n = new_node(key);
    n->height = lvl;
    for (int i = 0; i < lvl; i++) {
        n->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = n;
    }
    s->size++;
}
int sl_delete(SkipList *s, int key) {
    SkipNode *update[SL_MAX_LEVEL], *x = s->head;
    for (int i = s->level - 1; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->key < key) x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];
    if (!x || x->key != key) return 0;
    for (int i = 0; i < s->level; i++) {
        if (update[i]->forward[i] != x) break;
        update[i]->forward[i] = x->forward[i];
    }
    free(x);
    while (s->level > 1 && !s->head->forward[s->level - 1]) s->level--;
    s->size--;
    return 1;
}
void sl_print(SkipList *s) {
    for (int i = s->level - 1; i >= 0; i--) {
        printf("L%-2d: ", i);
        for (SkipNode *x = s->head->forward[i]; x; x = x->forward[i])
            printf("%d -> ", x->key);
        printf("NIL\n");
    }
}
/* Format: first line = level count; then one line per node: "key height" */
void sl_dump(SkipList *s, FILE *f) {
    fprintf(f, "%d\n", s->level);
    for (SkipNode *x = s->head->forward[0]; x; x = x->forward[0])
        fprintf(f, "%d %d\n", x->key, x->height);
}
void sl_free(SkipList *s) {
    SkipNode *x = s->head;
    while (x) { SkipNode *n = x->forward[0]; free(x); x = n; }
    free(s);
}
