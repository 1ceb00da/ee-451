#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 2

int n;
pthread_mutex_t shelf_lock;
pthread_cond_t shelf_cv;

struct consumer_param {
	int id;
} consumer_params[NUM_CONSUMERS];

void *Producer(void *arg) {
	int p = 0;
	
	while (p < 30) {
		pthread_mutex_lock(&shelf_lock);
		
		if (n == 0) {
			n = n + 2;
			p = p + 2;
			pthread_cond_broadcast(&shelf_cv);
			printf("Producer has put 2 cookies; # of cookies on the shelf changes from %d to %d.\n",(n-2),n);
		}

		else if (n < 9) {
			n = n + 2;
			p = p + 2;
			printf("Producer has put 2 cookies; # of cookies on the shelf changes from %d to %d.\n",(n-2),n);
		}
		
		else if (n == 9) {
			n = n + 1;
			p = p + 1;
			printf("Producer has put 1 cookies; # of cookies on the shelf changes from %d to %d.\n",(n-1),n);
		}
		
		else if (n == 10) {
			pthread_cond_wait(&shelf_cv, &shelf_lock);
		}
		
		pthread_mutex_unlock(&shelf_lock);
	}
}


void *Consumer(void *arg) {
	int c = 0;
	struct consumer_param *params = (struct consumer_param *) arg;
	
	while (c < 15) {
		pthread_mutex_lock(&shelf_lock);
		
		
		if (n == 0) {
			pthread_cond_wait(&shelf_cv, &shelf_lock);
		}
		
		else if (n > 0) {
			n = n - 1;
			c = c + 1;
			printf("Consumer %d has taken 1 cookies; # of cookies on the shelf changes from %d to %d.\n",params->id, (n+1), n);
		}
		
		pthread_mutex_unlock(&shelf_lock);
	}
}

int main (void) {
	int i;
	pthread_t *producers = (pthread_t *) malloc(sizeof(pthread_t *) * NUM_PRODUCERS);
	pthread_t *consumers = (pthread_t *) malloc(sizeof(pthread_t *) * NUM_CONSUMERS);
	pthread_mutex_init(&shelf_lock, NULL);
	pthread_cond_init(&shelf_cv, NULL);
		
	for (i=0; i < NUM_PRODUCERS; i++) {
		if (pthread_create(&producers[i], NULL, Producer, NULL) != 0) {
			printf("failed to create producer %d\n", i);
		}
	}
	
	for (i = 0; i < NUM_CONSUMERS; i++) {
		consumer_params[i].id = i;
		if (pthread_create(&consumers[i], NULL, Consumer, &consumer_params[i]) != 0) {
			printf("failed to create consumer %d\n", i);
		}
	}
	
	for (i = 0; i < NUM_PRODUCERS; i++) {
		if (pthread_join(producers[i], NULL) != 0) {
			printf("joing prodcuer %d failed\n", i);
		}
	}
	for (i = 0; i < NUM_CONSUMERS; i++) {
		if (pthread_join(consumers[i], NULL) != 0) {
			printf("joing consumer %d failed\n", i);
		}
	}
	
	free(producers);
	free(consumers);
	pthread_mutex_destroy(&shelf_lock);
	pthread_cond_destroy(&shelf_cv);
	
	return 0;
}
