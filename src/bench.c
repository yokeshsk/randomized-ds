/* Benchmarks: sorted / random / reverse inputs, randomised vs deterministic */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ds.h"

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }

static void fill(int *a, int n, const char *pat) {
    for (int i = 0; i < n; i++)
        a[i] = !strcmp(pat, "sorted") ? i : !strcmp(pat, "reverse") ? n - i : (int)(((unsigned)rand() << 8) ^ rand()) % 1000000007;
}

int main(void) {
    srand(12345);
    const int sizes[] = {1000, 2000, 5000, 10000, 20000, 40000};
    const char *pats[] = {"sorted", "random", "reverse"};
    FILE *f = fopen("results/bench.csv", "w");
    if (!f) { perror("results/bench.csv"); return 1; }
    fprintf(f, "algo,pattern,n,time_ms,extra\n");   /* extra = comparisons (sorts) or tree height */

    for (int p = 0; p < 3; p++) for (int s = 0; s < 6; s++) {
        int n = sizes[s];
        int *base = malloc(n * sizeof *base), *a = malloc(n * sizeof *a);
        fill(base, n, pats[p]);
        double t0;

        /* ----- sorting ----- */
        memcpy(a, base, n * sizeof *a); g_cmp = 0; t0 = now();
        det_quicksort(a, 0, n - 1);
        fprintf(f, "QuickSort (deterministic),%s,%d,%.4f,%lld\n", pats[p], n, (now()-t0)*1e3, g_cmp);

        memcpy(a, base, n * sizeof *a); g_cmp = 0; t0 = now();
        rand_quicksort(a, 0, n - 1);
        fprintf(f, "QuickSort (randomised),%s,%d,%.4f,%lld\n", pats[p], n, (now()-t0)*1e3, g_cmp);

        /* ----- insert n keys then search n keys ----- */
        t0 = now(); BstNode *b = NULL;
        for (int i = 0; i < n; i++) b = bst_insert(b, base[i]);
        for (int i = 0; i < n; i++) bst_search(b, base[i]);
        fprintf(f, "Plain BST,%s,%d,%.4f,%d\n", pats[p], n, (now()-t0)*1e3, bst_height(b));
        bst_free(b);

        t0 = now(); SkipList *sl = sl_create();
        for (int i = 0; i < n; i++) sl_insert(sl, base[i]);
        for (int i = 0; i < n; i++) sl_search(sl, base[i]);
        fprintf(f, "Skip List,%s,%d,%.4f,%d\n", pats[p], n, (now()-t0)*1e3, sl->level);
        sl_free(sl);

        t0 = now(); TreapNode *t = NULL;
        for (int i = 0; i < n; i++) t = tp_insert(t, base[i]);
        for (int i = 0; i < n; i++) tp_search(t, base[i]);
        fprintf(f, "Treap,%s,%d,%.4f,%d\n", pats[p], n, (now()-t0)*1e3, tp_height(t));
        tp_free(t);

        free(base); free(a);
        printf("done %-7s n=%d\n", pats[p], n); fflush(stdout);
    }
    fclose(f);
    printf("Wrote results/bench.csv\n");
    return 0;
}
