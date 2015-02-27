#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define num_threads 2

int a;
pthread_mutex_t mutex;
pthread_cond_t cv;

typedef struct {
	int thread_index;
} thread_param;

void *Thread_A(void *arg) {
	thread_param *params = (thread_param *) arg;
	printf("thread %d is created\n", params->thread_index);
	
	pthread_mutex_lock(&mutex);
	if (a < 5) {
		printf("Thread %d goes to sleep\n", params->thread_index);
		pthread_cond_wait(&cv, &mutex);
	}
	printf("thread %d wakes up\n", params->thread_index);
	pthread_mutex_unlock(&mutex);
}

void *Thread_B(void *arg) {
	thread_param *param = (thread_param *) arg;
	int i;
	
	printf("thread %d created\n", param->thread_index);
	for (i=0; i < 5; i++) {
		pthread_mutex_lock(&mutex);
		a++;
		
		printf("thread %d says a is %d\n", param->thread_index, a);
		
		if (a==5)
			pthread_cond_signal(&cv);
		pthread_mutex_unlock(&mutex);
	}
	
}

int main(void) {
	a = 0;
	int i;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cv, NULL);
	
	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t *) * num_threads);
	thread_param *thread_params = (thread_param *) malloc(sizeof(thread_param *) * num_threads);
	

	for (i = 0; i < num_threads; i++) 
		thread_params[i].thread_index = i;
	
	// create
	if (pthread_create(&threads[0], &attr, Thread_A, &thread_params[0]) != 0)
		printf("thread creating failed\n");
	if (pthread_create(&threads[1], &attr, Thread_B, &thread_params[1]) != 0)
		printf("thread creating failed\n");
	pthread_attr_destroy(&attr);
	
	// join
	for (i = 0; i < num_threads; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			printf("joing thread failed\n");
		}
	}
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cv);
	free(threads);
	free(thread_params);
	
	return 0;
}

