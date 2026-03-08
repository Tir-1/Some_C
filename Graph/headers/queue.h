#ifndef QUEUE_H
typedef struct Node{
	unsigned int info;
	struct Node* next;
} Node;

typedef struct Queue{
	Node* head;
	Node* tail;
} Queue;
int push(Queue* q, unsigned int value);
unsigned int pop(Queue* q);
int ifempty(Queue* q);
void free_q(Queue* q);
Queue* init();
#endif
