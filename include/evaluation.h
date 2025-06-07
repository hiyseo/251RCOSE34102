#ifndef EVALUATION_H
#define EVALUATION_H

#include "process.h"

double  calc_avg_waiting_time(Process* plist, int n);
double calc_avg_turnaround_time(Process* plist, int n);
void print_average_results(const char* algo_name, Process* plist, int n);

#endif