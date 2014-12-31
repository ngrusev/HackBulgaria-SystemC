#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

struct node {
	struct node *next;
	void *data;
};

struct queue {	
	struct node *head;
	struct node *tail;
	sem_t sem;
};

void init_queue(struct queue *q) {
	q->head = NULL;
	q->tail = NULL;
	sem_init(&q->sem, 1, 1);
}

void enqueue(struct queue *q, void *data) {
	struct node *newnode = malloc(sizeof(struct node));
	newnode->next = NULL;
	newnode->data = data;

	sem_wait(&q->sem);

	if (q->tail)
		q->tail->next = newnode;
	else
		q->head = newnode;
	q->tail = newnode;

	sem_post(&q->sem);
}

int dequeue(struct queue *q, void **pdata) {
	void *result;
	
	sem_wait(&q->sem);
	
	if (result = q->head) {
		*pdata = q->head->data;
		struct node* old = q->head;
		q->head = q->head->next;
		free(old);
		if (q->head == NULL)
			q->tail = NULL;
	}

	sem_post(&q->sem);
	return result != NULL;
}

struct queue *q;

void user(int *args) {
	int id = args[0];
	int start = args[1];
	int end = args[2];

	int i;
	for (i = start; i < end; i++) {
		enqueue(q, (void *)i);
		printf("thread %d: enq %d\n", id, i);
	}

	void *data;
	while(dequeue(q, &data))
		printf("thread %d: deq %d\n", id, (int)data);
}

int main() {
	q = malloc(sizeof(struct queue));
	init_queue(q);

	printf("number of threads: ");
	int n;
	scanf("%d", &n);
	
	pthread_t *threads = malloc(sizeof(pthread_t) * n);

	int i;
	for (i = 0; i < n; i++) {
		int *args = malloc(sizeof(int) * 3);
		args[0] = i;
		args[1] = i * 100;
		args[2] = args[1] + 100;
		pthread_create(threads + i, NULL, (void * (*) (void *)) &user, args);
	}

	for (i = 0; i < n; i++) {
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}
