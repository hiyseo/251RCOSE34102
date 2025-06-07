#include "queue.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

void enqueue(Queue* q, Process* proc){
    QueueNode* new_node = malloc(sizeof(QueueNode));
    new_node->proc = proc;
    new_node->next = NULL;

    if (q->rear){
        q->rear->next = new_node;
    } else {
        q->front = new_node;
    }

    q->rear = new_node;
}

Process* dequeue(Queue* q){
    if (!q->front){
        return NULL;
    }
    QueueNode* temp = q->front;
    Process *proc = temp->proc;
    q->front = temp->next;
    if (!q->front){
        q->rear = NULL;
    }
    free(temp);
    return proc;
}

int is_empty(Queue* q){
    return q->front == NULL;
}

void remove_from_queue(Queue* q, Process* p) {
    if (!q->front) return;

    QueueNode* prev = NULL;
    QueueNode* curr = q->front;

    while (curr) {
        if (curr->proc == p) {
            if (prev) prev->next = curr->next;
            else q->front = curr->next;

            if (q->rear == curr) q->rear = prev;

            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// reeady_queue의 상태를 출력
void print_ready_queue_state(int time) {
    printf("time %d: [Ready Queue] ", time);
    if (ready_queue.front == NULL) {
        printf("(empty)\n");
        return;
    }

    QueueNode* temp = ready_queue.front;
    while (temp != NULL) {
        printf("P%d(RT=%d, Pri=%d) ",
               temp->proc->pid,
               temp->proc->remaining_time,
               temp->proc->priority);
        temp = temp->next;
    }
    printf("\n");
}

// ready_queue에서 remaining_time이 0인 프로세스 제거
void clean_ready_queue() {
    QueueNode* prev = NULL;
    QueueNode* curr = ready_queue.front;

    while (curr != NULL) {
        if (curr->proc->remaining_time == 0) {
            // 삭제 대상
            QueueNode* to_delete = curr;
            if (prev == NULL) {
                // 맨 앞이면 front 이동
                ready_queue.front = curr->next;
            } else {
                prev->next = curr->next;
            }

            if (curr == ready_queue.rear) {
                ready_queue.rear = prev;
            }

            curr = curr->next;
            free(to_delete);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}