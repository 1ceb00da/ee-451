/* problem 2 parallel phw2 */
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

struct thread_data {
	int thread_id;
	int ITERS;
	double PI;
} thread_data_array[100];


void *compute_pi(void *data) {
	int ITERS;
	double a, b;
	double i, n;
	double t1, t2;
	double PI;
	struct thread_data *my_data;
	my_data = (struct thread_data *) data;
	
	printf("thread %d initializing...\n", my_data->thread_id);
	ITERS = my_data->ITERS;
	i = 0;
	n = 0;
	while(i < ITERS) {
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
	my_data->PI = PI;
	printf("thread %d finishing...\n", my_data->thread_id);
}


int main (int argc, char **argv) {

	const int m = 1e6;
	double PI[100];
	int i, rc, p;
	char *pstr;
	double load, PI_sum, PI_result;
	pthread_t threads[100];
	struct timespec start, stop;
	double time;
	
	for (i = 0; i < 100; i++)
		PI[i] = 0.0;
		
	pstr = argv[1];
	if (pstr == NULL) {
	    printf("Usage: %s <num threads integer>\n", argv[0]);
  		return 1;
    }
	p = atoi(pstr);
	
	load = (double)floor(m/(double)p);

	// start clock
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}

	for (i = 0; i < p; i++) {
		thread_data_array[i].thread_id = i;
		thread_data_array[i].ITERS = load;
		rc = pthread_create(&threads[i], NULL, compute_pi, (void *) &thread_data_array[i]);
		if (rc) { printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);}
	}
	
	for (i = 0; i < p; i++) {
		rc = pthread_join(threads[i], NULL);
		if (rc) { printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);}
		PI[i] = thread_data_array[i].PI;
	}
	
	PI_sum = 0.0;
	for (i = 0; i < p; i++)
		PI_sum += PI[i];
	
	PI_result = PI_sum / p;
	
	// stop clock
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	
	printf("PI = %lf\n", PI_result);
	printf("Execution time = %f sec,\n", time);
	return 0;
}
