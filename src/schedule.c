#include "schedule.h"
#include "config.h"
#include "queue.h"
#include "io.h"
#include <stdio.h>
#include <stdbool.h>

// ------------------------ FCFS ------------------------
void run_fcfs(Process* plist, int n){
    int time = 0; // 현재 시간
    int completed = 0; // 완료된 프로세스 수
    Process* current = NULL; // 현재 실행 중인 프로세스
    // int remaining_time = 0; // 남은 실행 시간

    while(completed<n){
        //처음 도착한 프로세스가 ready queue에 들어가는 경우
        for(int i=0; i<n; i++){
            if(plist[i].arrival_time == time){
                if(plist[i].initial_arrival_time == time){
                    printf("time %d: Process %d arrived\n", time, plist[i].pid);
                    enqueue(&ready_queue, &plist[i]);
                }
            }
        }

        update_waiting_queue(time); // I/O 완료 프로세스 복귀
        print_ready_queue_state(time);

        // 실행 중 프로세스가 I/O 요청하면 NULL 처리
        if (current) {
            bool io_requested = check_io_request(current, time); // I/O 요청 확인
            if (io_requested) {
                current = NULL;
            }
        }
        
        // CPU가 비어 있고 ready queue에 프로세스가 있으면 실행
        if (!current && !is_empty(&ready_queue)) {
            current = dequeue(&ready_queue);
            if (current->remaining_time > 0) {
                if (time > current->arrival_time) {
                    current->waiting_time += time - current->arrival_time; // 대기시간 계산
                }
                printf("time %d: Process %d starts executing\n", time, current->pid);
            } else {
                current = NULL; // 이미 끝난 프로세스는 건너뜀
            }
        }

        // 실행 중인 프로세스가 있으면 실행
        if (current) {
            current->remaining_time--;

            if (current->remaining_time == 0) {
                current->turnaround_time = time + 1 - current->initial_arrival_time;
                printf("time %d: Process %d completed(turnaround: %d, waiting: %d)\n",
                    time + 1, current->pid, current->turnaround_time, current->waiting_time);
                completed++;
                current = NULL;
            }
        } else {
            printf("time %d: CPU is idle\n", time);
        }

        time++;
    }
}

// ---------------- Shortest Job First (Non-preemptive) ----------------
void run_sjf(Process* plist, int n) {
    int time = 0, completed = 0;
    Process* current = NULL;

    while (completed < n) {
        //처음 도착한 프로세스가 ready queue에 들어가는 경우
        for(int i=0; i<n; i++){
            if(plist[i].arrival_time == time){
                if(plist[i].initial_arrival_time == time){
                    printf("time %d: Process %d arrived\n", time, plist[i].pid);
                    enqueue(&ready_queue, &plist[i]);
                }
            }
        }

        update_waiting_queue(time); // I/O 완료 프로세스 복귀
        print_ready_queue_state(time);

        // 실행 중 프로세스가 I/O 요청하면 NULL 처리
        if (current) {
            bool io_requested = check_io_request(current, time); // I/O 요청 확인
            if (io_requested) {
                current = NULL;
            }
        }

        // 현재 실행 중인 프로세스가 없고, ready queue가 비어있지 않다면 실행자 선택
        if (!current && !is_empty(&ready_queue)) {
            current = get_shortest_process_and_remove(time);
            if (current) {
                if (time > current->arrival_time) { // current->arrival_time: 프로세스가 ready queue에 들어온 시간
                    current->waiting_time += time - current->arrival_time;
                }
                printf("time %d: Process %d starts executing\n", time, current->pid);
            }
        }

        // 프로세스 실행
        if (current) {
            current->remaining_time--;

            if (current->remaining_time == 0) {
                current->turnaround_time = time + 1 - current->initial_arrival_time;
                printf("time %d: Process %d completed(turnaround: %d, waiting: %d)\n",
                       time + 1, current->pid, current->turnaround_time, current->waiting_time);
                completed++;
                current = NULL;
            }
        } else {
            printf("time %d: CPU is idle\n", time);
        }
        time++;
    }
}

// ---------------- Shortest Job First (Preemptive) ----------------
void run_preemptive_sjf(Process* plist, int n) {
    int time = 0, completed = 0;
    Process* current = NULL;

    while (completed < n) {
        //처음 도착한 프로세스가 ready queue에 들어가는 경우
        for(int i=0; i<n; i++){
            if(plist[i].arrival_time == time){
                if(plist[i].initial_arrival_time == time){
                    printf("time %d: Process %d arrived\n", time, plist[i].pid);
                    enqueue(&ready_queue, &plist[i]);
                }
            }
        }

        update_waiting_queue(time);
        clean_ready_queue();
        print_ready_queue_state(time);

        // 실행 중 프로세스가 I/O 요청하면 NULL 처리
        if (current) {
            bool io_requested = check_io_request(current, time);
            if (io_requested) {
                current = NULL;
            }
        }

        // 매 시간마다 가장 짧은 job 선택
        Process* shortest = get_shortest_process_in_ready_queue(time);

        if (shortest && shortest != current) { // 새로 선택된 프로세스가 현재 실행 중인 프로세스와 다를 때
            if (current && current->remaining_time > 0) {
                enqueue(&ready_queue, current);  // 기존 실행자 다시 큐에 넣음
            }
            remove_from_queue(&ready_queue, shortest);
            current = shortest;
            if (time > current->arrival_time) {
                current->waiting_time += time - current->arrival_time;
            }
            printf("time %d: Process %d starts executing\n", time, current->pid);
        }

        // 현재 실행 중인 프로세스 실행
        if (current) {
            current->remaining_time--;

            if (current->remaining_time == 0) {
                current->turnaround_time = time + 1 - current->initial_arrival_time;
                printf("time %d: Process %d completed(turnaround: %d, waiting: %d)\n",
                    time + 1, current->pid, current->turnaround_time, current->waiting_time);
                completed++;
                current = NULL;
            }
        } else {
            printf("time %d: CPU is idle\n", time);
        }
        time++;
    }
}

// ---------------- Priority Scheduling (Non-preemptive) ----------------
void run_priority(Process* plist, int n) {
    int time = 0, completed = 0;
    Process* current = NULL;

    while (completed < n) {
        //처음 도착한 프로세스가 ready queue에 들어가는 경우
        for(int i=0; i<n; i++){
            if(plist[i].arrival_time == time){
                if(plist[i].initial_arrival_time == time){
                    printf("time %d: Process %d arrived\n", time, plist[i].pid);
                    enqueue(&ready_queue, &plist[i]);
                }
            }
        }

        update_waiting_queue(time);
        print_ready_queue_state(time);

        // 실행 중 프로세스가 I/O 요청하면 NULL 처리
        if (current) {
            bool io_requested = check_io_request(current, time);
            if (io_requested) {
                current = NULL;
            }
        }

        // 실행 중인 프로세스가 없으면 우선순위 기반으로 선택
        if (!current && !is_empty(&ready_queue)) {
            Process* highest = get_highest_priority_process();
            if (highest) {
                remove_from_queue(&ready_queue, highest);
                current = highest;

                if (time > current->arrival_time) {
                    current->waiting_time += time - current->arrival_time;
                }

                printf("time %d: Process %d starts executing (Priority %d)\n",
                       time, current->pid, current->priority);
            }
        }

        if (current) {
            current->remaining_time--;

            if (current->remaining_time == 0) {
                current->turnaround_time = time + 1 - current->initial_arrival_time;
                printf("time %d: Process %d completed(turnaround: %d, waiting: %d)\n",
                       time + 1, current->pid, current->turnaround_time, current->waiting_time);
                completed++;
                current = NULL;
            }
        } else {
            printf("time %d: CPU is idle\n", time);
        }

        time++;
    }
}

// ---------------- Priority Scheduling (Preemptive) ----------------
void run_preemptive_priority(Process* plist, int n) {
    int time = 0, completed = 0;
    Process* current = NULL;

    while (completed < n) {
        //처음 도착한 프로세스가 ready queue에 들어가는 경우
        for(int i=0; i<n; i++){
            if(plist[i].arrival_time == time){
                if(plist[i].initial_arrival_time == time){
                    printf("time %d: Process %d arrived\n", time, plist[i].pid);
                    enqueue(&ready_queue, &plist[i]);
                }
            }
        }

        update_waiting_queue(time);
        print_ready_queue_state(time);

        // 실행 중 프로세스가 I/O 요청하면 NULL 처리
        if (current) {
            bool io_requested = check_io_request(current, time);
            if (io_requested) {
                current = NULL;
            }
        }

        // 매 시간마다 우선순위 가장 높은 프로세스 선택
        Process* highest = get_highest_priority_process();

        // 새로 선택된 프로세스의 priority가 더 높을 때 선점
        if (highest && (!current || highest->priority < current->priority)) {
            if (current && current->remaining_time > 0) {
                enqueue(&ready_queue, current);
            }
            remove_from_queue(&ready_queue, highest);
            current = highest;

            if (time > current->arrival_time) {
                current->waiting_time += time - current->arrival_time;
            }

            printf("time %d: Process %d starts executing (Priority %d)\n",
                time, current->pid, current->priority);
        }

        if (current) {
            current->remaining_time--;
            if (current->remaining_time == 0) {
                current->turnaround_time = time + 1 - current->initial_arrival_time;
                printf("time %d: Process %d completed(turnaround: %d, waiting: %d)\n",
                       time + 1, current->pid, current->turnaround_time, current->waiting_time);
                completed++;
                current = NULL;
            }
        } else {
            printf("time %d: CPU is idle\n", time);
        }
        time++;
    }
}

// ---------------- Round Robin (Preemptive) ----------------
void run_rr(Process* plist, int n) {
    int time = 0, completed = 0;
    Process* current = NULL;
    int quantum = 0;

    while (completed < n) {
        //처음 도착한 프로세스가 ready queue에 들어가는 경우
        for(int i=0; i<n; i++){
            if(plist[i].arrival_time == time){
                if(plist[i].initial_arrival_time == time){
                    printf("time %d: Process %d arrived\n", time, plist[i].pid);
                    enqueue(&ready_queue, &plist[i]);
                }
            }
        }

        update_waiting_queue(time);
        print_ready_queue_state(time);

        // I/O 요청 처리 (진행 중인 프로세스가 있을 경우)
        if (current) {
            bool io_requested = check_io_request(current, time);
            if (io_requested) {
                current = NULL;
                quantum = 0;
            }
        }

        // Quantum 소진 or current 없음 → 교체
        if ((!current || quantum == 0) && !is_empty(&ready_queue)) {
            if (current && current->remaining_time > 0) {
                if (quantum == 0) {
                    printf("time %d: Time quantum expired. Switching from Process %d\n", time, current->pid);
                }
                enqueue(&ready_queue, current);  // quantum 끝났지만 아직 안 끝남
            }

            current = dequeue(&ready_queue);
            quantum = TIME_QUANTUM;

            if (time > current->arrival_time) {
                current->waiting_time += time - current->arrival_time;
            }

            printf("time %d: Process %d starts executing\n", time, current->pid);
        }

        // 실행
        if (current) {
            current->remaining_time--;
            quantum--;

            // 프로세스 종료
            if (current->remaining_time == 0) {
                current->turnaround_time = time + 1 - current->initial_arrival_time;
                printf("time %d: Process %d completed(turnaround: %d, waiting: %d)\n",
                       time + 1, current->pid, current->turnaround_time, current->waiting_time);
                completed++;
                current = NULL;
                quantum = 0;
            }
        } else {
            printf("time %d: CPU is idle\n", time);
        }

        time++;
    }
}

