#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode {
	QuadTreeNode* elem;
	struct QueueNode* next;
} QueueNode;

typedef struct Queue {
	QueueNode* front;
	QueueNode* rear;
	long size;
} Queue;

// Crearea unei cozi
Queue* createQueue(void) {
	Queue* q = (Queue*)calloc(1, sizeof(Queue));
	return q;
}

// Verifica daca coada este goala
int isQueueEmpty(Queue* q) {
	if (q == NULL || q->rear == NULL) return 1;
	return 0;
}

// Adauga un element in coada
void enqueue(Queue* q, QuadTreeNode* elem) {
	if (q == NULL) return;
	QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
	node->next = NULL;
	/* daca coada este goala */
	if (isQueueEmpty(q) == 1) {
		node->elem = elem;
		q->rear = q->front = node;
		q->size++;
		return;
	}
	/* daca coada are cel putin un element */
	node->elem = elem;
	q->rear->next = node;
	// "node" devine ultimul nod din coada
	q->rear = node;
	q->size++;
}

// Returneaza valoarea primului element din coada
QuadTreeNode* front(Queue* q) {
	return q->front->elem;
}

// Extrage un element din coada (dupa regula FIFO)
void dequeue(Queue* q) {
	if (q == NULL) return;
	// coada este goala
	if (isQueueEmpty(q) == 1) {
		return;
	}
	// coada contine doar un element
	if (q->front == q->rear) {
		free(q->front);
		q->front = q->rear = NULL;
		q->size--;
		return;
	}
	// coada contine mai multe elemente
	QueueNode* aux = q->front;
	q->front = q->front->next;
	free(aux);
	q->size--;
}

// Dealocare memorie pentru coada
void destroyQueue(Queue* q) {
	QueueNode* iter = q->front;
	while (iter != NULL) {
		QueueNode* aux = iter;
		iter = iter->next;
		free(aux);
	}
	free(q);
}

#endif	// _QUEUE_H_
