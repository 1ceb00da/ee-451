#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define num_threads 5

int a;
pthread_mutex_t mutex;

typedef struct {
	int thread_index;
} thread_param;

void *Thread_Fn(void *arg) {
	thread_param *param = (thread_param *) arg;
	int k;
	
	for (k=0; k < 10; k++) {
		pthread_mutex_lock(&mutex);
		a++;
		printf("%d says: a is %d now\n", param->thread_index, a);
		pthread_mutex_unlock(&mutex);
	}
}

int main(void) {
	a = 0;
	int i;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_mutex_init(&mutex, NULL);
	
	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t *) * num_threads);
	thread_param *params = (thread_param *) malloc(sizeof(thread_param *) * num_threads);
	
	// create threads
	for (i = 0; i < num_threads; i++) {
		params[i].thread_index = i;
		if (pthread_create(&threads[i], &attr, Thread_Fn, (void *) &params[i]) != 0) {
			printf("creating thread failed\n");
		}
	}
	
	pthread_attr_destroy(&attr);
	
	// join
	for (i = 0; i < num_threads; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			printf("joing thread failed\n");
		}
	}
	
	pthread_mutex_destroy(&mutex);
	free(threads);
	free(params);
	
	return 0;
}

