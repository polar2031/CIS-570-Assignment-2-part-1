#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define MAX_WAIT 5

typedef int buffer_item;

void *producer(void*);
void *consumer(void*);
void insert_item(int*);
void remove_item(int*);

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
buffer_item buffer[BUFFER_SIZE];
int head;
int tail;

int main(int argc, char *argv[])
{
	int producerNumber;
	int consumerNumber;
	int sleepTime;
	int i;
	pthread_t pro, con;
	//transfer input
	if(argc == 4){
		producerNumber = atoi(argv[1]);
		consumerNumber = atoi(argv[2]);
		sleepTime = atoi(argv[3]);
	}
	else{
		printf("Need 3 argument.\n");
		return 0;
	}
	//initial
	srand(time(NULL));
	head = 0;
	tail = 0;
	pthread_mutex_init(&mutex, PTHREAD_MUTEX_TIMED_NP);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);

	for(i = 0; i < producerNumber; i++){
		pthread_create(&pro, NULL, &producer, NULL);
	}
	for(i = 0; i < consumerNumber; i++){
		pthread_create(&con, NULL, &consumer, NULL);
	}
	sleep(sleepTime);
	return 0;
}

void *producer(void *arg){
	int product;
	while(1){
		sleep(rand()%MAX_WAIT);
		product = rand()%20;
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		insert_item(&product);
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		printf("produce: %d\n", product);
	}
	return NULL;
}

void *consumer(void *arg){
	int product;
	while(1){
		sleep(rand()%MAX_WAIT);
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		remove_item(&product);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		printf("\t\tconsume: %d\n", product);
	}
	return NULL;
}

void insert_item(int *product){
	buffer[head] = *product;
	head = (head + 1) % BUFFER_SIZE;
	return;
}

void remove_item(int *product){
	*product = buffer[tail];
	tail = (tail + 1) % BUFFER_SIZE;
	return;
}
