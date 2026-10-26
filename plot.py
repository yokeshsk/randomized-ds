"""Reads results/*.csv|txt written by the C programs and draws all charts."""
import csv, math, collections
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

R = "results/"
rows = list(csv.DictReader(open(R + "bench.csv")))
data = collections.defaultdict(lambda: collections.defaultdict(list))   # (algo,pat) -> n -> ...
for r in rows:
    data[(r["algo"], r["pattern"])]["n"].append(int(r["n"]))
    data[(r["algo"], r["pattern"])]["t"].append(float(r["time_ms"]))
    data[(r["algo"], r["pattern"])]["x"].append(float(r["extra"]))

PATS = ["sorted", "random", "reverse"]
def grid(pairs, ycol, ylabel, title, fname, log=True):
    fig, axs = plt.subplots(1, 3, figsize=(15, 4.2), sharey=True)
    for ax, pat in zip(axs, PATS):
        for algo, style in pairs:
            d = data[(algo, pat)]
            ax.plot(d["n"], d[ycol], style, label=algo, marker="o", ms=4)
        ax.set_title(f"{pat} input"); ax.set_xlabel("n"); ax.grid(alpha=.3)
        if log: ax.set_yscale("log")
    axs[0].set_ylabel(ylabel); axs[0].legend()
    fig.suptitle(title); fig.tight_layout(); fig.savefig(R + fname, dpi=130); plt.close(fig)

QS = [("QuickSort (deterministic)", "r-"), ("QuickSort (randomised)", "g-")]
DS = [("Plain BST", "r-"), ("Skip List", "b-"), ("Treap", "g-")]
grid(QS, "t", "time (ms, log)", "QuickSort: deterministic vs randomised pivot", "quicksort_time.png")
grid(QS, "x", "comparisons (log)", "QuickSort comparisons - random pivot avoids O(n^2)", "quicksort_comparisons.png")
grid(DS, "t", "time for n inserts + n searches (ms, log)", "Plain BST vs Skip List vs Treap", "ds_time.png")
grid([("Plain BST", "r-"), ("Treap", "g-")], "x", "tree height (log)", "Tree height: randomised priorities keep the treap balanced", "tree_height.png")

# ---- empirical proof on sorted input: comparisons vs theory ----
fig, ax = plt.subplots(figsize=(6.5, 4.5))
d1, d2 = data[("QuickSort (deterministic)", "sorted")], data[("QuickSort (randomised)", "sorted")]
ns = d1["n"]
ax.plot(ns, d1["x"], "ro-", label="deterministic (measured)")
ax.plot(ns, [n*(n-1)/2 for n in ns], "r--", alpha=.5, label="n(n-1)/2  (worst case)")
ax.plot(ns, d2["x"], "go-", label="randomised (measured)")
ax.plot(ns, [1.39*n*math.log2(n) for n in ns], "g--", alpha=.5, label="1.39 n log2 n  (expected)")
ax.set_yscale("log"); ax.set_xlabel("n (SORTED input)"); ax.set_ylabel("comparisons")
ax.set_title("Empirical proof: random pivot avoids the worst case"); ax.legend(); ax.grid(alpha=.3)
fig.tight_layout(); fig.savefig(R + "worst_case_proof.png", dpi=130); plt.close(fig)

# ---- Skip list levels ----
lines = open(R + "skiplist.txt").read().split("\n")
L = int(lines[0]); nodes = [tuple(map(int, l.split())) for l in lines[1:] if l.strip()]
fig, ax = plt.subplots(figsize=(11, 3 + .35*L))
for i, (k, h) in enumerate(nodes):
    for lv in range(h):
        ax.add_patch(plt.Rectangle((i - .35, lv - .35), .7, .7, color=plt.cm.viridis(lv / max(L, 1))))
    ax.text(i, -1, k, ha="center", fontsize=8)
    ax.plot([i, i], [-.5, h - .65], color="gray", lw=.6, zorder=0)
ax.set_xlim(-1, len(nodes)); ax.set_ylim(-1.6, L); ax.set_yticks(range(L)); ax.set_yticklabels([f"Level {i}" for i in range(L)])
ax.set_xticks([]); ax.set_title("Skip List: node height assigned by coin flips (each level ~ half the nodes)")
fig.tight_layout(); fig.savefig(R + "skiplist_levels.png", dpi=130); plt.close(fig)

# ---- Treap structure ----
T = {}
for l in open(R + "treap.txt"):
    k, p, a, b = l.split(); T[int(k)] = (int(p), int(a), int(b))
root = next(iter(T))
pos, depth, counter = {}, {}, [0]
def walk(k, d):
    if k == -1: return
    walk(T[k][1], d + 1); pos[k] = counter[0]; depth[k] = d; counter[0] += 1; walk(T[k][2], d + 1)
walk(root, 0)
fig, ax = plt.subplots(figsize=(12, 1.6 + .9*(max(depth.values())+1)))
for k, (p, a, b) in T.items():
    for c in (a, b):
        if c != -1: ax.plot([pos[k], pos[c]], [-depth[k], -depth[c]], "k-", lw=.8, zorder=1)
for k in T:
    ax.scatter(pos[k], -depth[k], s=420, c="#9ecae1", edgecolors="k", zorder=2)
    ax.text(pos[k], -depth[k], k, ha="center", va="center", fontsize=8, zorder=3)
ax.axis("off"); ax.set_title("Treap built from SORTED input 1..31 (random priorities keep it balanced)")
fig.tight_layout(); fig.savefig(R + "treap_structure.png", dpi=130); plt.close(fig)
print("Charts saved in results/")
