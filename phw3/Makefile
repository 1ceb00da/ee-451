all: p1a p1b p2

p1a: p1a.c
	gcc -lrt p1a.c -o p1a -lm -lpthread

p1b: p1b.c
	gcc -lrt p1b.c -o p1b -lm -lpthread

p2: p2.c
	gcc -lrt p2.c -o p2 -lm -lpthread

clean:
	rm -f p1a p1b p2
