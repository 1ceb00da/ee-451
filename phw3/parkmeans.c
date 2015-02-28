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


// Helper functions - begin//
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
// Helper functions - end//

// data defn for each thread
struct thread_data {
    unsigned char *arr;
    int to, from;
    float *means_t;
    int thread_id;
    int num[K];
    float sum[K];
} thread_data_array[8];


// k means algo
void *KMeans(void *data) {

    struct thread_data *arg = (struct thread_data *) data;
    int from = arg->from;
    int to = arg->to;
    unsigned char *arr = arg->arr;
    int *cluster = malloc(sizeof(int *) * (to-from));
    float means[6];
    
    int i, i2, _w, m;
    float temp[K];
	
    printf("thread %d starting..\n", arg->thread_id);
    printf("data fro thread %d\n",arg->thread_id);
    printf("to,from,means %d %d %f %f %f %f %f %f\n",arg->to, arg->from, means[0],means[1],means[2],means[3],means[4],means[5],);
    
    
    // get data from struct
     // arr, arr_size
     // arr is the row this thread deals with
     // i.e. its the ith row
     // arr is a pointer to the main array 'a'
     // from and to indicate the bounds that this thread dels with
    means[0] = 0.0;
    means[1] = 65.0;
    means[2] = 100.0;
    means[3] = 125.0;
    means[4] = 190.0;
    means[5] = 255.0;
    for (i = 0; i < 50; i++) {
        //findClosestMeans(cluster, arr, from, to, means, K);
	    for (i2 = 0;  i2 <= (to-from); i2++) {
    	    for (m = 0; m < K; m++) {
    	        temp[m] = (float)fabs(arr[i2+from] - means[m]);
    	    }
    	    cluster[i] = min(temp, K);
    	    _w = min(temp, K);
    	    arg->sum[_w] += arr[i+from];
    	    arg->num[_w] += 1;
    	    arg->cluster[i2+from] = cluster[i];
		}

    
    //computeMeans(means, K, arr, from, to, cluster);    
    }

    // return means
	
    printf("thread %d finishing..\n", arg->thread_id);
    
}


