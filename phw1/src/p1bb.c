/* problem 1 a */
/* Aditya Dhulipala */
/* EE/CSCI 451 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void multiply_matrices(double **c, int c_start, int c_fin, double **a, int a_start, int a_fin, double **b, int b_start, int b_fin) {
    printf("need to mutliply %d %d\n", c_start, c_fin);
}

int main(int argc,char **argv){
		int i, j, k;
		struct timespec start, stop; 
		double time;
		long unsigned int n = 4096; // matrix size is n*n
		
		char *bstr = argv[1];
		int b, m;
		
		
		int bi, bj, bk;
		double temp;
		
		
		double **A = (double**) malloc (sizeof(double*)*n);
		double **B = (double**) malloc (sizeof(double*)*n);
		double **C = (double**) malloc (sizeof(double*)*n);
		
		
		int jj, kk, N, s;
		
		N = n;

		
		// parse block size from command line; default is 8
		if (bstr == NULL) {
		    printf("Usage: %s <block_size integer>\n", argv[0]);
  		    return 1;
      	}
      	b = *bstr - '0';
  	    m = n / b;
		s = b;
		for (i=0; i<n; i++) {
			A[i] = (double*) malloc(sizeof(double)*n);
			B[i] = (double*) malloc(sizeof(double)*n);
			C[i] = (double*) malloc(sizeof(double)*n);
		}
		
		for (i=0; i<n; i++){
			for(j=0; j< n; j++){
				A[i][j]=i;
				B[i][j]=i+j;
				C[i][j]=0;			
			}
		}		
		
		if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
		
		// Your code goes here //
		// Matrix C = Matrix A * Matrix B //	
		//*******************************//
		for(jj=0;jj<N;jj+= s){
        for( kk=0;kk<N;kk+= s){
                for( i=0;i<N;i++){
                        for( j = jj; j<((jj+s)>N?N:(jj+s)); j++){
                                temp = 0;
                                for(k = kk; k<((kk+s)>N?N:(kk+s)); k++){
                                        temp += a[i][k]*b[k][j];
                                }
                                c[i][j] += temp;
                        }
                }
        }
}
		
		
		//*******************************//
		
		if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
		time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		
		printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);
		printf("C[100][100]=%lf\n", C[100][100]);
		
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
