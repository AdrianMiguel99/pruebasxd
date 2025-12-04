#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"

// Inicializa las colas internas
void scheduler_init(void);

// Agrega un proceso listo según prioridad
void scheduler_add_ready(PCB *p);

// Devuelve el proceso en ejecución
PCB* scheduler_current(void);

// Cambia al siguiente proceso listo (cooperativo)
void scheduler_switch(PCB *next);

// Selecciona el siguiente proceso listo
PCB* scheduler_pick_next(void);

// Ejecuta procesos listos hasta que la cola quede vacía
void scheduler_run(void (*print)(const char*));

// Cede la CPU y reprograma el siguiente proceso
void scheduler_yield(void);

#endif /* SCHEDULER_H */
