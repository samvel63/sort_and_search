CC = gcc
CFLAGS = -g -std=c99 -Wno-unused-result -pipe -O3

all:  main sort

main: main.o table.o 
	$(CC) $^ -o $@

sort: sort.o table.o
	$(CC) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f main *.o