# Randomised QuickSort, Skip List & Treap (in C)

DAA project: randomised algorithms and data structures vs their deterministic counterparts.

| Structure | Randomness | Expected cost | Deterministic counterpart |
|---|---|---|---|
| Randomised QuickSort | random pivot | O(n log n) on *any* input | last-element pivot (O(n^2) on sorted input) |
| Skip List | coin-flip node heights | O(log n) search/insert/delete | plain BST |
| Treap | random heap priorities | O(log n) expected height | plain BST |

## Build & run
```bash
make            # builds ./demo and ./bench
./demo          # correctness tests + prints skip list levels
./bench         # benchmarks -> results/bench.csv  (~10 s)
python3 plot.py # needs matplotlib -> charts in results/
# or all at once:  make run
```

## Results (see `results/`)
- `worst_case_proof.png` – comparisons on sorted input: deterministic follows n(n-1)/2, randomised follows 1.39 n log2 n
- `quicksort_time.png`, `quicksort_comparisons.png` – sorted / random / reverse
- `ds_time.png`, `tree_height.png` – Plain BST vs Skip List vs Treap
- `skiplist_levels.png`, `treap_structure.png` – visualisations

## Files
`src/quicksort.c` `src/skiplist.c` `src/treap.c` `src/bst.c` – implementations · `src/ds.h` – API ·
`src/main.c` – tests/demo · `src/bench.c` – benchmark · `plot.py` – charts

## Real-world use
Skip lists: Redis sorted sets · Treaps: competitive programming · Randomised pivot: production sort libraries.
