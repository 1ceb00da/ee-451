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

void initKMeans(float *means) {
    means[0] = 0.0;
    means[1] = 65.0;
    means[2] = 100.0;
    means[3] = 125.0;
    means[4] = 190.0;
    means[5] = 255.0;
    return;
}


struct thread_data {
    unsigned char *arr;
    int to, from;
    float *thread_means;
    int *cluster;
    int thread_id;
    int num[6];
    float sum[6];
} thread_data_array[8];


void *KMeans(void *data) {

    struct thread_data *arg = (struct thread_data *) data;
    int from = arg->from;
    int to = arg->to;
    unsigned char *arr = arg->arr;
    
    int thread_id = arg->thread_id;
    
    int load = to-from+1;
    
    int *cluster = (int *) malloc(sizeof(int *) * (load));
    float *thread_means = (float *) malloc(sizeof(float *) * K);
    float means[6];
    
    int iter, i, _w, m;
    float temp[K];
	
    printf("thread %d starting..\n", arg->thread_id);
    
    // init means
    means[0] = 0.0;
    means[1] = 65.0;
    means[2] = 100.0;
    means[3] = 125.0;
    means[4] = 190.0;
    means[5] = 255.0;
    arg->thread_means = thread_means;
    
    for (iter = 0; iter < 50; iter++) {
	    for (i = from;  i <= to; i++) {
    	    for (m = 0; m < K; m++) {
    	        temp[m] = (float)fabs(dist(arr[i],means[m]));
    	    }
    	    cluster[i%load-1] = min(temp, K);
    	    _w = min(temp, K);
    	    arg->sum[_w] += arr[i];
    	    arg->num[_w] += 1;
    	    arg->cluster[i] = cluster[i%load-1];
    	    
		}
		for (m = 0; m < K; m++) {
			thread_means[m] = arg->sum[m] / arg->num[m];
		}
    }
	
    printf("thread %d finishing..\n", arg->thread_id);
    //free(cluster);
}


int main(int argc, char** argv){
    int i, iter, k, rc;
	struct timespec start, stop; 
	double time;
    FILE *fp;

  	unsigned char *a = (unsigned char*) malloc (sizeof(unsigned char)*h*w);
    int *cluster = (int *)malloc(sizeof(int *)*h*w);
    int *temp_cluster;
    float *final_means = (float *)malloc(sizeof(float *)*K);
    float **thread_means;
    float *temp_means;
    
    int p, left, right;
    int ii,j;
    
    float *sum;
    int *num;
    int m;
    char *pstr;
    
    double load;
    pthread_t threads[8];
    
    k = 6;
    
    pstr = argv[1];
	if (pstr == NULL) {
	    printf("Usage: %s <num threads integer>\n", argv[0]);
  		return 1;
    }
	p = atoi(pstr);
	load = floor((w*h)/(double)p);
	thread_means = (float **)malloc(sizeof(float **) * p);
	
	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not opern file\n");
		return 1;
	}
	fread(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);
    
	// measure the start time here
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	

    // ------------------ //
	for (i = 0; i < p; i++) {
		
	
	    left = i * load;
	    right = left + load - 1;
	    
	    // formulate thread data
	    thread_data_array[i].arr = a;
	    thread_data_array[i].from = left;
	    thread_data_array[i].to = right;
	    thread_data_array[i].thread_id = i;
	    thread_data_array[i].cluster = cluster;
	    
	    rc = pthread_create(&threads[i], NULL, KMeans, (void *) &thread_data_array[i]);
	    if (rc) { 
	        printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);
	    }
	}
	
	for (i = 0; i < p; i++) {
	    rc = pthread_join(threads[i], NULL);
	    sum = thread_data_array[i].sum;
	    num = thread_data_array[i].num;
	    thread_means[i] = thread_data_array[i].thread_means;
	    if (rc) { printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);
	    }
	}
	

	// combine all means
	for (i = 0; i < p; i++) {
		temp_means = thread_data_array[i].thread_means;
		for (m = 0; m < K; m++) {
			final_means[m] += temp_means[m];
		}
	}
	
	for (i = 0; i < K; i++) {
		final_means[i] = final_means[i] / p;
	}
	
	
 	// Map each pixel to its centroid value
	for (i = 0; i < w*h; i++) {
	    // Review -- this line from prev hw
	    a[i] = final_means[cluster[i]];
	} 
		// measure the end time here
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}
	
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not opern file\n");
		return 1;
	}	
	fwrite(a, sizeof(unsigned char),w*h, fp);
    fclose(fp);
	
	// End of custom code //
	// ------------------ //
	

	
	// print out the execution time here
    time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
    printf("Execution time = %f sec\n", time);	
	
	

    
    free(a);
    free(cluster);
    free(final_means);
    
    return 0;



}

