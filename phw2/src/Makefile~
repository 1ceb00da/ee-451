all: p1 p2serial p2parallel

p1: p1.c
	gcc -lrt p1.c -o p1 -lm -lpthread

p2serial: p2serial.c
	gcc -lrt p2serial.c -o p2serial -lm -lpthread

p2parallel: p2parallel.c
	gcc -lrt p2parallel.c -o p2parallel -lm -lpthread

p3: p3.c
	gcc -lrt p3.c -o p3 -lm -lpthread

clean:
	rm -f p1 p2serial p2parallel p3
