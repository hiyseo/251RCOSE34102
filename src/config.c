#include "config.h"
#include <stdio.h>

// 큐 및 전역 시간
Queue ready_queue;
Queue waiting_queue;
int current_time;

void init_system(){
    ready_queue.front = ready_queue.rear = NULL;
    waiting_queue.front = waiting_queue.rear = NULL;
    current_time = 0;
}