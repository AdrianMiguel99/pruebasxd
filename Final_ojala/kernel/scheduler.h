#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"

void scheduler_init(void);
void scheduler_add_ready(PCB *p);
PCB* scheduler_pick_next(void);
PCB* scheduler_current(void);
void scheduler_switch(PCB *next);
void scheduler_run(void (*print)(const char*));
void scheduler_yield(void);

#endif /* SCHEDULER_H */
