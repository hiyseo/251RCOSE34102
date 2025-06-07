#ifndef IO_H
#define IO_H

#include "process.h"
#include <stdbool.h>

bool check_io_request(Process* current, int current_time);
void update_waiting_queue(int current_time);
Process* get_shortest_process_and_remove(int time);
Process* get_shortest_process_in_ready_queue(int time);
Process* get_highest_priority_process();

#endif