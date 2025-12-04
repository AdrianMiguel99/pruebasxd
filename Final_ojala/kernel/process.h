#ifndef PROCESS_H
#define PROCESS_H

#include "pcb.h"

void process_init(void);
int process_create(const char *name, proc_prio_t prio);
void process_list(void (*print)(const char*));
int process_kill(int pid);
int process_run(int pid, void (*print)(const char*));
PCB* process_get(int pid);

#endif /* PROCESS_H */
