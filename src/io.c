#include "io.h"
#include "config.h"
#include "queue.h"
#include <stdio.h>
#include <stdbool.h>

bool check_io_request(Process* current, int current_time) {
    if (current->remaining_time <= 0) return false;

    // 현재 프로세스가 실행된 시간
    int executed_time = current->burst_time - current->remaining_time;

    for (int i = 0; i < current->io_count; i++) {
        if (current->io_request_times[i] == executed_time &&
            current->io_requested_flags[i] == 0) {
            
            printf("time %d: Process %d requests I/O(duration %d)\n", 
                   current_time, current->pid, current->io_burst_times[i]);

            current->arrival_time = current_time + current->io_burst_times[i];
            current->io_requested_flags[i] = 1;
            enqueue(&waiting_queue, current);

            return true;
        }
    }
    return false;
}

void update_waiting_queue(int current_time){
    int size=0;

    // size 계산
    QueueNode* temp = waiting_queue.front;
    while(temp!=NULL){
        size++;
        temp=temp->next;
    }


    for(int i=0; i<size; i++){
        Process* p = dequeue(&waiting_queue);
        if(p->arrival_time <= current_time){ // I/O가 완료되어 ready queue로 돌아갈 수 있는 경우
            if(p->remaining_time>0){
                enqueue(&ready_queue, p);
                printf("time %d: Process %d completes I/O and returns to Ready Queue\n", current_time, p->pid);
            }
        } else {
            enqueue(&waiting_queue, p);
        }
    }
}

// 헬퍼 함수: ready_queue에서 가장 짧은 작업 선택 + dequeue
Process* get_shortest_process_and_remove(int time) {
    Process* shortest = NULL;
    QueueNode* temp = ready_queue.front;

    // ready_queue에서 가장 짧은 remaining_time을 가진 프로세스 찾기
    while (temp) {
        if (temp->proc->remaining_time > 0 &&
            (!shortest || temp->proc->remaining_time < shortest->remaining_time)) {
            shortest = temp->proc;
        }
        temp = temp->next;
    }
    if (shortest) {
        printf("time %d: Selected P%d as shortest job (RT=%d)\n", time, shortest->pid, shortest->remaining_time);
        remove_from_queue(&ready_queue, shortest);
    }
    return shortest;
}

// Ready Queue 중 가장 짧은 Job 반환 (삭제는 안 함)
Process* get_shortest_process_in_ready_queue(int time) {
    Process* shortest = NULL;
    QueueNode* temp = ready_queue.front;

    // ready_queue에서 가장 짧은 remaining_time을 가진 프로세스 찾기
    while (temp) {
        if (temp->proc->remaining_time > 0 &&
            (!shortest || temp->proc->remaining_time < shortest->remaining_time)) {
            shortest = temp->proc;
        }
        temp = temp->next;
    }

    if (shortest){
        printf("time %d: Selected P%d as shortest job (RT=%d)\n", time, shortest->pid, shortest->remaining_time);
    }

    return shortest;
}

// 우선순위 기준 (숫자가 작을수록 높은 우선순위)로 프로세스 선택
Process* get_highest_priority_process() {
    Process* highest = NULL;
    QueueNode* temp = ready_queue.front;

    // ready_queue에서 가장 높은 우선순위를 가진 프로세스 찾기
    while (temp) {
        if (temp->proc->remaining_time > 0 &&
            (!highest || temp->proc->priority < highest->priority)) {
            highest = temp->proc;
        }
        temp = temp->next;
    }
    return highest;
}