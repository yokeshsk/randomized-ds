/* Demo + correctness tests + visualisation data export */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "ds.h"

static int sorted_ok(int *a, int n) { for (int i = 1; i < n; i++) if (a[i-1] > a[i]) return 0; return 1; }

int main(void) {
    srand((unsigned)time(NULL));

    /* --- QuickSort test --- */
    int n = 5000, *a = malloc(n * sizeof *a);
    for (int i = 0; i < n; i++) a[i] = rand() % 100000;
    rand_quicksort(a, 0, n - 1);
    assert(sorted_ok(a, n)); free(a);
    printf("[OK] Randomised QuickSort sorts correctly\n");

    /* --- Skip List test + demo --- */
    SkipList *sl = sl_create();
    for (int i = 1; i <= 20; i++) sl_insert(sl, i * 5);
    printf("\nSkip List (20 keys, levels are random):\n"); sl_print(sl);
    assert(sl_search(sl, 50) && !sl_search(sl, 51));
    assert(sl_delete(sl, 50) && !sl_search(sl, 50));
    printf("[OK] Skip List insert/search/delete\n");
    FILE *f = fopen("results/skiplist.txt", "w"); if (f) { sl_dump(sl, f); fclose(f); }
    sl_free(sl);

    /* --- Treap test + demo: SORTED insertion, still balanced --- */
    TreapNode *t = NULL;
    for (int i = 1; i <= 31; i++) t = tp_insert(t, i);
    printf("\nTreap after inserting 1..31 in SORTED order: height = %d (plain BST would be 31)\n", tp_height(t));
    assert(tp_search(t, 17) && !tp_search(t, 99));
    t = tp_delete(t, 17); assert(!tp_search(t, 17));
    printf("[OK] Treap insert/search/delete\n");
    f = fopen("results/treap.txt", "w"); if (f) { tp_dump(t, f); fclose(f); }
    tp_free(t);

    printf("\nAll tests passed. Run ./bench to generate results/bench.csv\n");
    return 0;
}
