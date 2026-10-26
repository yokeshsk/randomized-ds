CC     = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=199309L
SRC    = src/quicksort.c src/skiplist.c src/treap.c src/bst.c

all: demo bench

demo: src/main.c $(SRC) src/ds.h
	$(CC) $(CFLAGS) -o $@ src/main.c $(SRC)
bench: src/bench.c $(SRC) src/ds.h
	$(CC) $(CFLAGS) -o $@ src/bench.c $(SRC)

run: all
	mkdir -p results && ./demo && ./bench && python3 plot.py
clean:
	rm -f demo bench results/*.csv results/*.txt results/*.png
