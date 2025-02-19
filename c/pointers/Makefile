CC=cc
CFLAGS=

.PHONY: clean

all:
	$(CC) $(CFLAGS) -ansi sort_lines.c input.c array.c alloc.c qsort.c -o sort_lines

clean:
	rm sort_lines || echo Already clean
