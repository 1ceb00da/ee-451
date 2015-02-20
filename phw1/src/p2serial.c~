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

	const int MAX_ITERARTIONS =  100;
	int i, n;
	double a,b;
	double t1,t2;
	double PI;
	
	i = MAX_ITERATIONS;
	
	n = 0;
	while(i > 0) {
		a = generate_rand();
		b = generate_rand();
		t1 = a-0.5;
		t2 = b-0.5;
		
		if ((t1*t1) + (t2*t2) <= 0.25) {
			n++;
		}
		i--;
	}
	
	PI = n/(MAX_ITERATIONS * 4);
	printf("PI = %lf\n", PI);
	
	return 0;
}
