#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define h  800 
#define w  800

#define iterations_total 1e6
#define k 4

#define input_file  "input.raw"
#define output_file "output.raw"

void findClosestCentroids(int *idx, unsigned char *a, int n, float *centroids, int k) {
    int i, j;
    for (i = 0;  i < n; i++) {
        for (j = 0; j < k; j++) {
            // temp[j] = dist(a[i], centroids[j]);
        }
        
    }
}

void computeMeans(float *centroids, unsigned char *a, int *idx, int k_) {

}

void initKMeans(float *m) {
    m[0] = 0.0;
    m[1] = 85.0;
    m[2] = 170.0;
    m[3] = 255.0;
    return;
}

int main(int argc, char** argv){
    int i, iter;
	struct timespec start, stop; 
	double time;
    FILE *fp;

  	unsigned char *a = (unsigned char*) malloc (sizeof(unsigned char)*h*w);
    int *idx = (int *)malloc(sizeof(int *)*h*w);
    float *centroids = (float *)malloc(sizeof(float *)*h*w);
    
    
	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not opern file\n");
		return 1;
	}
	fread(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);
    
	// measure the start time here
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	
	//  Your code goes here
	initKMeans(centroids);
	
	for (iter = 0; iter < iterations_total; iter++) {
		//findClosestCentroids(idx, a, h*w, centroids, k);
		//computeMeans(centrods, a, idx, k);
		
		
	}	
	//
	
	// measure the end time here
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}
	
	// print out the execution time here
    time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
    printf("Execution time = %f sec\n", time);	
	
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not opern file\n");
		return 1;
	}	
	fwrite(a, sizeof(unsigned char),w*h, fp);
    fclose(fp);
    
    return 0;
}
