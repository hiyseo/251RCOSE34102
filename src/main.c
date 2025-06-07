#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "config.h"
#include "schedule.h"
#include "evaluation.h"

int main(){
    init_system(); // 시스템 초기화

    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process* base = create_processes(n); // 프로세스 생성
    Process* fcfs_list = clone_processes(base, n);
    Process* sjf_list = clone_processes(base, n);
    Process* psjf_list = clone_processes(base, n);
    Process* prio_list = clone_processes(base, n);
    Process* pprio_list = clone_processes(base, n);
    Process* rr_list = clone_processes(base, n);

    // 스케줄링
    printf("\n================ FCFS Scheduling ================\n");
    run_fcfs(fcfs_list, n);
    print_average_results("FCFS", fcfs_list, n);

    printf("\n================ Non-preemptive SJF Scheduling ================\n");
    run_sjf(sjf_list, n);
    print_average_results("Non-preemptive SJF", sjf_list, n);

    printf("\n================ Preemptive SJF Scheduling ================\n\n");
    run_preemptive_sjf(psjf_list, n);
    print_average_results("Preemptive SJF", psjf_list, n);

    printf("\n================ Non-preemptive Priotiry Scheduling ================\n");
    run_priority(prio_list, n);
    print_average_results("Non-preemptive Priotiry", prio_list, n);

    printf("\n================ Preemptive Priority Scheduling ================\n");
    run_preemptive_priority(pprio_list, n);
    print_average_results("Preemptive Priority", pprio_list, n);

    printf("\n========= Round Robin Scheduling (Time Quantum = %d) =========\n", TIME_QUANTUM);
    run_rr(rr_list, n);
    print_average_results("Round Robin", rr_list, n);


    // 메모리 해제
    free_processes(base, n);
    free_processes(fcfs_list, n);
    free_processes(sjf_list, n);
    free_processes(psjf_list, n);
    free_processes(prio_list, n);
    free_processes(pprio_list, n);
    free_processes(rr_list, n);


    return 0;
}