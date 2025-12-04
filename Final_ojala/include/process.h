#ifndef PROCESS_H
#define PROCESS_H

#include "pcb.h"

// Inicializa la tabla global de procesos
void process_init(void);

// Crea un proceso con una prioridad dada. Usa una entrada de prueba si no se pasa otra.
int process_create(const char *name, proc_prio_t prio);

// Lista procesos usando la función de impresión provista
void process_list(void (*print)(const char*));

// Ejecuta un proceso de forma cooperativa
int process_run(int pid, void (*print)(const char*));

// Marca un proceso como zombie
int process_kill(int pid);

// Devuelve el PCB asociado a un PID
PCB* process_get(int pid);

#endif /* PROCESS_H */
