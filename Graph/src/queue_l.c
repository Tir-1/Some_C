#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int push(Queue* q, unsigned int value){
	Node* x = malloc(sizeof(Node));
	if(x == NULL){return 1;}
	x->next = NULL;
	x->info = value;
	if(q->head == NULL){
		q->head = x;
		q->tail = x;
	}
	else{
		(q->tail)->next = x;
		q->tail = x;
	}
	return 0;
}

unsigned int pop(Queue* q){
	Node* x = q->head;
	q->head = (q->head)->next;
	unsigned int data = x->info;
	free(x);
	return data;
}

Queue* init(){
	Queue* q = malloc(sizeof(Queue));
	if(q == NULL)return NULL;
	q->head = NULL;
	q->tail = NULL;
	return q;
}

void free_q(Queue* q){
	while(q->head != NULL){
		Node* ptr = q->head;
		q->head = (q->head)->next;
		free(ptr);
	}
	free(q);
}

int ifempty(Queue* q){
	if(q->head == NULL)return 1;
	return 0;	
}
