# ifndef PROCESS_H
# define PROCESS_H

typedef struct {
    int pid; 
    int arrival_time;
    int initial_arrival_time;
    int burst_time;
    int priority;

    int* io_request_times;
    int* io_burst_times;
    int io_count;
    int* io_requested_flags; // 크기 io_count만큼의 배열

    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;

Process* create_processes(int n);
Process* clone_processes(Process* original, int n);
void free_processes(Process* plist, int n);

#endif