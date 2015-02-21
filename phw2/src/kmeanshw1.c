#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define h  800
#define w  800

#define iterations_total 30

#define input_file  "input(1).raw"
#define output_file "output.raw"

float dist(unsigned char a, float b) {
    a = (float)a;
    return ((float)fabs(a-b));
}

int min(float *ar, int n) {
    int min = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (ar[i] < ar[min])
            min = i;
    }
    return min;
}

void findClosestCentroids(int *cluster, unsigned char *a, int n, float *centroids, int k) {
    int i, m;
    float *temp = malloc(k * sizeof(float));
    for (i = 0;  i < n; i++) {
        for (m = 0; m < k; m++) {
            temp[m] = dist(a[i], centroids[m]);
        }
        cluster[i] = min(temp, k);
        
    }
    free(temp);
}

void computeMeans(
    float *centroids, int K,
    unsigned char *a, int n, 
    int *cluster) {
    
    int k, i;
    float sum;
    int count;
    
    for (k = 0; k < K; k++) {
        sum = 0.0f;
        count = 0;
        for (i = 0; i < n; i++) {
            if (cluster[i] == k) {
                sum += a[i];
                count++;
            }
        centroids[k] = (sum / count);
        }
    }

}

void initKMeans(float *m) {
    m[0] = 0.0;
    m[1] = 85.0;
    m[2] = 170.0;
    m[3] = 255.0;
    return;
}

int main(int argc, char** argv){
    int i, iter, k;
	struct timespec start, stop; 
	double time;
    FILE *fp;

  	unsigned char *a = (unsigned char*) malloc (sizeof(unsigned char)*h*w);
    int *cluster = (int *)malloc(sizeof(int *)*h*w);
    float *centroids = (float *)malloc(sizeof(float *)*k);
    
    k = 4;
    
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
		findClosestCentroids(cluster, a, h*w, centroids, k);
		computeMeans(
		    centroids, k,
		    a, w*h,
		    cluster);
	}	
	
	// Map each pixel to its centroid value
	for (i = 0; i < (w*h); i++) {
	    a[i] = centroids[cluster[i]];
	}
	
	
	
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
    
    free(a);
    free(cluster);
    free(centroids);
    
    return 0;
}
