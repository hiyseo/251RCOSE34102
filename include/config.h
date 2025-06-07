#ifndef CONFIG_H
#define CONFIG_H

#include "queue.h" 
#include "process.h"

extern Queue ready_queue;
extern Queue waiting_queue;
extern int current_time;

void init_system();


#endif
