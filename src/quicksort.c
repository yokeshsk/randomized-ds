#include <stdlib.h>
#include "ds.h"

long long g_cmp = 0;

static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

static int partition(int *a, int lo, int hi) {   /* Lomuto, pivot at a[hi] */
    int p = a[hi], i = lo - 1;
    for (int j = lo; j < hi; j++) {
        g_cmp++;
        if (a[j] <= p) swap(&a[++i], &a[j]);
    }
    swap(&a[i + 1], &a[hi]);
    return i + 1;
}

/* Deterministic: last element is pivot -> O(n^2) on sorted input */
void det_quicksort(int *a, int lo, int hi) {
    if (lo >= hi) return;
    int p = partition(a, lo, hi);
    det_quicksort(a, lo, p - 1);
    det_quicksort(a, p + 1, hi);
}

/* Randomised: random pivot -> expected O(n log n) on ANY input.
   Recurse on the smaller half, loop on the larger (stack depth O(log n)). */
void rand_quicksort(int *a, int lo, int hi) {
    while (lo < hi) {
        int r = lo + rand() % (hi - lo + 1);
        swap(&a[r], &a[hi]);
        int p = partition(a, lo, hi);
        if (p - lo < hi - p) { rand_quicksort(a, lo, p - 1); lo = p + 1; }
        else                 { rand_quicksort(a, p + 1, hi); hi = p - 1; }
    }
}
