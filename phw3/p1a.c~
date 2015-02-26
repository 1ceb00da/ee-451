#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 2

int n;
pthread_mutex_t shelf_lock;

void *Producer(void *arg) {
	int p = 0;
	
	while (p < 30) {
		pthread_mutex_lock(&shelf_lock);

		if (n < 9) {
			n = n + 2;
			p = p + 2;
			printf("producer 'if' msg\n");
		}
		
		else if (n == 9) {
			n = n + 1;
			p = p + 1;
			printf("prducer 'else' msg\n");
		}
		
		pthread_mutex_unlock(&shelf_lock);
	}
}


void *Consumer(void *arg) {
	int c = 0;

	while (c < 15) {
		pthread_mutex_lock(&shelf_lock);
		
		if (n > 0) {
			n = n - 1;
			c = c + 1;
			printf("consumer msg\n");
		}
		
		pthread_mutex_unlock(&shelf_lock);
	}
}

int main (void) {
	int i;
	pthread_t *producers = (pthread_t *) malloc(sizeof(pthread_t *) * NUM_PRODUCERS);
	pthread_t *consumers = (pthread_t *) malloc(sizeof(pthread_t *) * NUM_CONSUMERS);
	pthread_mutex_init(&shelf_lock, NULL);
		
	for (i=0; i < NUM_PRODUCERS; i++) {
		if (pthread_create(&producers[i], NULL, Producer, NULL) != 0) {
			printf("failed to create producer %d\n", i);
		}
	}
	
	for (i = 0; i < NUM_CONSUMERS; i++) {
		if (pthread_create(&consumers[i], NULL, Consumer, NULL) != 0) {
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
	
	return 0;
}
