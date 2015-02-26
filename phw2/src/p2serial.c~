/* problem 2 serial phw2 */
/* Aditya Dhulipala */
/* EE/CSCI 451 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

double generate_rand() {
	return (double) rand() / (double) RAND_MAX;
}

int main () {

	const int MAX_ITERATIONS =  1e6;
	double i, n;
	double a,b;
	double t1,t2;
	double PI;
	struct timespec start, stop;
	double time;
	
	// start clock
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	
	i = 0;
	n = 0;
	while(i < MAX_ITERATIONS) {
		a = generate_rand();
		b = generate_rand();
		t1 = a-0.5;
		t2 = b-0.5;
		
		if ((t1*t1) + (t2*t2) <= 0.25) {
			n++;
		}
		i++;
	}
	
	PI = n/i * 4;
	
	// clock stop
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

	printf("PI = %lf\n", PI);
	printf("Execution time = %f sec,\n", time);	
	
	return 0;
}

