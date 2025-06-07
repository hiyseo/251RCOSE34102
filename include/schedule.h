#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "process.h"

#define TIME_QUANTUM 2

void run_fcfs(Process* plist, int n);
void run_sjf(Process* plist, int n);
void run_priority(Process* plist, int n);
void run_rr(Process* plist, int n);
void run_preemptive_sjf(Process* plist, int n);
void run_preemptive_priority(Process* plist, int n);

#endif