#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <pthread.h>

#define h  1024
#define w  1024

#define iterations_total 50

#define input_file  "input(1).raw"
#define output_file "output.raw"


const int K = 6;

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

void findClosestMeans(int *cluster, unsigned char *a, int from, int to, float *means, int k) {
    int i, m;
    float *temp = malloc(k * sizeof(float));
    for (i = from;  i <= to; i++) {
        for (m = 0; m < k; m++) {
            temp[m] = dist(a[i], means[m]);
        }
        cluster[i] = min(temp, k);
        
    }
    free(temp);
}


void computeMeans(
    float *means, int K,
    unsigned char *a, int from, int to, 
    int *cluster) {
    
    int k, i;
    float sum;
    int count;
    
    for (k = 0; k < K; k++) {
        sum = 0.0f;
        count = 0;
        for (i = from; i <= to; i++) {
            if (cluster[i] == k) {
                sum += a[i];
                count++;
            }
        means[k] = (sum / count);
        }
    }

}

void initKMeans(float *m) {
    m[0] = 0.0;
    m[1] = 65.0;
    m[2] = 100.0;
    m[3] = 125.0;
    m[4] = 190.0;
    m[5] = 255.0;
    return;
}


struct thread_data {
    unsigned char *arr;
    int to, from;
    float *computed_means;
} thread_data_array[8];



void *KMeans(void *data) {

    struct thread_data *arg = (struct thread_data *) data;
    int from = arg->from;
    int to = arg->to;
    unsigned char *arr = arg->arr;
    int *cluster = malloc(sizeof(int *) * (to-from));
    float *means = (float *)malloc(sizeof(float *)*K);
    
    int i;
    
    
    // get data from struct
     // arr, arr_size
     // arr is the row this thread deals with
     // i.e. its the ith row
     // arr is a pointer to the main array 'a'
     // from and to indicate the bounds that this thread dels with
	initKMeans(means);
    for (i = 0; i < 50; i++) {
        findClosestMeans(cluster, arr, from, to, means, K);
        computeMeans(means, K, arr, from, to, cluster);
    }
    
    // return means
    arg->computed_means = means;
    
 	// Map each pixel to its centroid value
	for (i = 0; i < (to-from); i++) {
	    // Review -- this line from prev hw
	    // a[i] = means[cluster[i]];
	} 
    
}

int main(int argc, char** argv){
    int i, iter, k, rc;
	struct timespec start, stop; 
	double time;
    FILE *fp;

  	unsigned char *a = (unsigned char*) malloc (sizeof(unsigned char)*h*w);
    int *cluster = (int *)malloc(sizeof(int *)*h*w);
    float *final_means = (float *)malloc(sizeof(float *)*k);
    
    int p, left, right;
    char *pstr;
    
    pthread_t threads[8];
    
    k = 4;
    
    pstr = argv[1];
	if (pstr == NULL) {
	    printf("Usage: %s <num threads integer>\n", argv[0]);
  		return 1;
    }
	p = atoi(pstr);
	
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
    // ------------------ //
	for (i = 0; i < p; i++) {
	    left = i * w;
	    right = ((i+1) * h) - 1;
	    
	    // formulate thread data
	    thread_data_array[i].arr = a;
	    thread_data_array[i].from = left;
	    thread_data_array[i].to = right;

	    rc = pthread_create(&threads[i], NULL, KMeans, (void *) &thread_data_array[i]);
	    if (rc) { 
	        printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);
	    }
	}
	
	
	for (i = 0; i < p; i++) {
	    rc = pthread_join(threads[i], NULL);
	    if (rc) { printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);
	    }
	}
	
	
	// TODO: combine all means
	
	// TODO: write to output file
	
	// End of custom code //
	// ------------------ //
	
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
    free(final_means);
    
    return 0;



}

