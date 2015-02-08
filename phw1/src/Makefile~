all: p1a p1b p2

p1a: p1a.c
	gcc -lrt p1a.c -o p1a

p1b: p1b.c
	gcc -lrt p1b.c -o p1b

p2: problem2.c
	gcc -lrt problem2.c -o p2

clean:
	rm -f p1a p1b p2
