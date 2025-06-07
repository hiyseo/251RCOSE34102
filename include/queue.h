#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

typedef struct QueueNode{
    Process* proc;
    struct QueueNode* next;
} QueueNode;

typedef struct{
    QueueNode* front;
    QueueNode* rear;
} Queue;

void enqueue(Queue* q, Process* proc);
Process* dequeue(Queue* q);
int is_empty(Queue* q);

void remove_from_queue(Queue* q, Process* p);
void print_ready_queue_state(int time);
void clean_ready_queue();

#endif