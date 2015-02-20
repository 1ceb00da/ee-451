/* problem 1 phw2 */
/* Aditya Dhulipala */
/* EE/CSCI 451 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <math.h>


struct thread_data {
	int thread_id;
	double **C;
	double **A;
	double **B;
	
	int a,b;
	int n;
};
void printmat(double **A, int n) {
	int i,j;
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				printf("%lf ",A[i][j]);
			}
			printf("\n");
		}
}

void *MultiplyMatrices(void *data) {
	struct thread_data *my_data;
	my_data = (struct thread_data *) data;
	
	double **C;
	double **B;
	double **A;
	
	int a,b,n;
	int i,k,j;
	
	double sum;
	int count = 0;
	
	C = my_data->C;
	A = my_data->A;
	B = my_data->B;
	
	a = my_data->a;
	b = my_data->b;
	n = my_data->n;
	
	printf("thread %d executing...\n", my_data->thread_id);
	
	for (i = 0; i < n; i++) {
		for (j = a; j <=b; j++) {
		sum = 0.0;
			for (k = 0; k < n; k++) {
				sum += A[i][k] * B[k][j];
				count++;
			}
			C[i][j] = sum;
		}
	}

	printf("thread %d finsihed... %d iterations executed when a,b = %d,%d\n", my_data->thread_id, count, a,b);
	
}

struct thread_data thread_data_array[256];


int main(int argc, char **argv){
		int i, j, k;
		struct timespec start, stop; 
		double time;
		long unsigned int n = 4096; // matrix size is n*n
		
		double **A = (double**) malloc (sizeof(double*)*n);
		double **B = (double**) malloc (sizeof(double*)*n);
		double **C = (double**) malloc (sizeof(double*)*n);
		
		char *pstr;
		char numthreads;
		
		double load;
		
		
		pthread_t threads[256];
		int *thread_ids[256];
		int rc;
		
		for (i=0; i<n; i++) {
			A[i] = (double*) malloc(sizeof(double)*n);
			B[i] = (double*) malloc(sizeof(double)*n);
			C[i] = (double*) malloc(sizeof(double)*n);
		}

		// parse number of threads from command line.
		pstr = argv[1];
		if (pstr == NULL) {
		    printf("Usage: %s <num threads integer>\n", argv[0]);
  		    return 1;
      	}
      	
      	numthreads = atoi(pstr);
		
		load = floor(n/(double)numthreads);
		
		printf("load per thread = n/p = %lf\n",load);
		for (i=0; i<n; i++){
			for(j=0; j< n; j++){
				A[i][j]=i;
				B[i][j]=i+j;
				C[i][j]=0;			
			}
		}
		
		// test

		//printmat(A,n);
		//printf("\ntop A --- bot B\n");
		//printmat(B,n);
		///////
		
		// thread definitions	
		if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
		for (i = 0; i < numthreads; i++) {
			thread_data_array[i].thread_id = i;
			thread_data_array[i].A = A;
			thread_data_array[i].B = B;
			thread_data_array[i].C = C;
			thread_data_array[i].a = (int) (i * (load));
			thread_data_array[i].b = (int) (((i+1) * (load)) -1);
			thread_data_array[i].n = n;
			
			rc = pthread_create(&threads[i], NULL, MultiplyMatrices, (void *) &thread_data_array[i] );
				if (rc) { printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);}		
		}

		for(i=0; i<numthreads; i++) {	  
				rc = pthread_join(threads[i], NULL);
				if (rc) { printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);}
			}
		
		if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
		time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		
		printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);		
		printf("C[100][100]=%lf\n", C[2][3]);
		
		
		
		//printmat(C,n);
		
		
		// release memory
		for (i=0; i<n; i++) {
			free(A[i]);
			free(B[i]);
			free(C[i]);
		}
		free(A);
		free(B);
		free(C);
		return 0;
}
