#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <pcb.h>

/* ============================================================
   SCHEDULER DE MULTIPLES COLAS (MULTI-LEVEL QUEUE)
   Tres colas:
     0 = Prioridad alta
     1 = Prioridad media
     2 = Prioridad baja
   ============================================================ */

#define NUM_QUEUES 3

/* ============================================================
   API DEL SCHEDULER
   ============================================================ */

/* Inicializa las colas de prioridad */
void scheduler_init(void);

/* Inserta un proceso en la cola según su prioridad */
void scheduler_add(pcb_t *p);

/* Saca y retorna el siguiente proceso a ejecutar */
pcb_t* scheduler_next(void);

/* Probar manualmente la cola completa (opción del menú) */
void scheduler_test(void);

#endif /* SCHEDULER_H */
