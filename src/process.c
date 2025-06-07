// 랜덤 프로세스 생성 (PID, ARRIVAL TIME, CPU BURST TIME, IO REQUEST TIME, PRIORITY)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"

Process* create_processes(int n){
    srand(time(NULL));
    Process* plist = malloc(sizeof(Process) * n);
    for (int i=0; i<n; i++){
        plist[i].pid = i+1; // 프로세스 ID
        plist[i].arrival_time = rand() % 10; // ready 큐에 도착하는 시간 (0~9)
        plist[i].initial_arrival_time = plist[i].arrival_time; // 초기 도착 시간 저장
        plist[i].burst_time = 5 + rand()%10; // CPU burst 시간(5~14)
        plist[i].priority = 1 + rand() % n; // 우선순위(1~n)
        plist[i].io_count = 1 + rand()%3; // I/O 요청(1~3개)
        plist[i].io_request_times = malloc(sizeof(int) * plist[i].io_count); // I/O 요청 시간
        plist[i].io_burst_times = malloc(sizeof(int) * plist[i].io_count); // I/O burst 시간
        plist[i].io_requested_flags = calloc(plist[i].io_count, sizeof(int)); // 0으로 초기화

        int used[20] = {0}; // 중복 시간 방지용
        int assigned = 0;
        while (assigned < plist[i].io_count) {
            int t = rand() % plist[i].burst_time;
            if (!used[t]) {
                used[t] = 1;
                plist[i].io_request_times[assigned] = t+1;
                plist[i].io_burst_times[assigned] = 1 + rand()%3;
                assigned++;
            }
        }

        // 초기 설정
        plist[i].remaining_time = plist[i].burst_time;
        plist[i].waiting_time = 0;
        plist[i].turnaround_time = 0;

        // 기본 정보 출력
        printf("[Process %d] Arrival Time: %d, CPU Burst Time: %d, Priority: %d, IO Count: %d\n", 
            plist[i].pid, plist[i].arrival_time, plist[i].burst_time, plist[i].priority, plist[i].io_count);

        // I/O 정보 출력
        for (int j=0; j<plist[i].io_count; j++) {
            printf("    └─ IO Request %d: Time = %d, Burst = %d\n", 
                j+1, plist[i].io_request_times[j], plist[i].io_burst_times[j]);
        }
    }
    return plist;
}

Process* clone_processes(Process* original, int n) {
    Process* copy = malloc(sizeof(Process) * n);
    for (int i = 0; i < n; i++) {
        copy[i] = original[i];

        // 깊은 복사를 위해 동적 할당
        copy[i].io_request_times = malloc(sizeof(int) * copy[i].io_count);
        copy[i].io_burst_times = malloc(sizeof(int) * copy[i].io_count);
        copy[i].io_requested_flags = calloc(copy[i].io_count, sizeof(int));

        for (int j = 0; j < copy[i].io_count; j++) {
            copy[i].io_request_times[j] = original[i].io_request_times[j];
            copy[i].io_burst_times[j] = original[i].io_burst_times[j];
        }

        copy[i].remaining_time = copy[i].burst_time;
        copy[i].waiting_time = 0;
        copy[i].turnaround_time = 0;
        copy[i].arrival_time = copy[i].initial_arrival_time;
    }
    return copy;
}

void free_processes(Process* plist, int n) {
    for (int i = 0; i < n; i++) {
        free(plist[i].io_request_times);
        free(plist[i].io_burst_times);
        free(plist[i].io_requested_flags);
    }
    free(plist); // 프로세스 배열 자체
}