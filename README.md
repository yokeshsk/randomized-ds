# Randomised QuickSort, Skip List & Treap (in C)

A Design and Analysis of Algorithms (DAA) project that implements three randomised algorithms and data structures in C. It benchmarks each against a deterministic counterpart on sorted, random and reverse-sorted input.

## Why randomisation?

Deterministic algorithms have a fixed worst case. For example, QuickSort with a fixed pivot takes O(n²) on already-sorted data, and a plain BST degenerates into a linked list. Adding randomness makes the *expected* running time good on **every** input, because no fixed input can reliably trigger the worst case.

| Structure | Source of randomness | Expected cost | Deterministic counterpart |
|---|---|---|---|
| Randomised QuickSort | random pivot | O(n log n) on any input | last-element pivot, O(n²) on sorted input |
| Skip List | coin-flip node heights | O(log n) search, insert, delete | plain BST |
| Treap | random heap priorities | O(log n) expected height | plain BST |

## Features

- Randomised QuickSort with a random pivot (recursion depth bounded to O(log n))
- Empirical proof that the random pivot avoids the worst case on sorted input
- Skip List with probabilistic level assignment (p = 1/2, max 16 levels)
- Treap: a BST ordered by key and a max-heap ordered by random priority, balanced with rotations
- Benchmarks of all structures on sorted, random and reverse data
- Visualisations of Skip List levels and Treap structure

## Project structure

```
├── Makefile
├── plot.py                 # draws all charts from results/
├── src/
│   ├── ds.h                # shared declarations
│   ├── quicksort.c         # deterministic + randomised QuickSort
│   ├── skiplist.c          # Skip List
│   ├── treap.c             # Treap
│   ├── bst.c               # plain BST (deterministic baseline)
│   ├── main.c              # tests + demo
│   └── bench.c             # benchmark -> results/bench.csv
└── results/                # CSV data and generated charts
```

## Requirements

- `gcc` (or any C11 compiler) and `make`
- Python 3 with matplotlib: `pip install matplotlib`

## Build and run

```bash
make            # build ./demo and ./bench
./demo          # correctness tests + prints Skip List levels
./bench         # run benchmarks -> results/bench.csv (about 10 s)
python3 plot.py # generate charts in results/
```

Or run everything at once with `make run`.

## Results

| Chart | What it shows |
|---|---|
| `results/worst_case_proof.png` | On sorted input, deterministic QuickSort follows n(n-1)/2 comparisons, while randomised follows about 1.39·n·log₂n |
| `results/quicksort_time.png` | QuickSort running time on sorted, random and reverse input |
| `results/quicksort_comparisons.png` | QuickSort comparison counts |
| `results/ds_time.png` | Time for n inserts and n searches: Plain BST vs Skip List vs Treap |
| `results/tree_height.png` | Plain BST height vs Treap height |
| `results/skiplist_levels.png` | Skip List level structure |
| `results/treap_structure.png` | Treap built from sorted input 1..31 |

**Key observations**
- Deterministic QuickSort and the plain BST slow down quadratically on sorted and reverse input.
- Randomised QuickSort, the Skip List and the Treap stay near O(n log n) on all three input types.
- A Treap built from sorted input 1..31 has height 9, where a plain BST has height 31.

## Complexity

| Operation | Skip List | Treap | Plain BST (worst) |
|---|---|---|---|
| Search | O(log n) expected | O(log n) expected | O(n) |
| Insert | O(log n) expected | O(log n) expected | O(n) |
| Delete | O(log n) expected | O(log n) expected | O(n) |

QuickSort comparisons are O(n log n) expected for the random pivot, and O(n²) worst case for the fixed pivot.

## Real-world applications

- **Skip Lists:** Redis sorted sets
- **Treaps:** competitive programming, ordered sets with split and merge
- **Randomised QuickSort:** production sort libraries (random or median-of-3 pivots)
