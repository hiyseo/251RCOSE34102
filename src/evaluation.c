#include <stdio.h>
#include "evaluation.h"

double calc_avg_waiting_time(Process* plist, int n){
    double total=0;
    printf("\n======== Waiting Times ========\n");
    for(int i=0; i<n; i++){
        printf("Waiting Time of Process %d: %d\n", plist[i].pid, plist[i].waiting_time);
        total+=plist[i].waiting_time;
    }
    return total/n;
}


double calc_avg_turnaround_time(Process* plist, int n) {
    double total = 0;
    printf("\n======== Turnaround Times ========\n");
    for (int i = 0; i < n; i++) {
        printf("Turnaround Time of Process %d: %d\n", plist[i].pid, plist[i].turnaround_time);
        total += plist[i].turnaround_time;
    }
    return total / n;
}

void print_average_results(const char* algo_name, Process* plist, int n) {
    double avg_wait = calc_avg_waiting_time(plist, n);
    double avg_turn = calc_avg_turnaround_time(plist, n);
    printf("\n======== [%s] Average Times ========\n", algo_name);
    printf("Average Waiting Time: %.2f\n",avg_wait);
    printf("Average Turnaround Time: %.2f\n", avg_turn);
}