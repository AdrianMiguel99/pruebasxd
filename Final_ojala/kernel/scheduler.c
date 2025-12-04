#include "pcb.h"
#include "queue.h"
#include "scheduler.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// -------------------------------
//    TRES COLAS DE LISTOS
// -------------------------------

static queue_t ready_high;
static queue_t ready_med;
static queue_t ready_low;

// Proceso actualmente ejecutándose
static PCB *current_process = NULL;


// -------------------------------
//     Inicializar scheduler
// -------------------------------
void scheduler_init(void) {
    queue_init(&ready_high);
    queue_init(&ready_med);
    queue_init(&ready_low);
}


// -------------------------------
//   Encolar un proceso según prioridad
// -------------------------------
void scheduler_add_ready(PCB *p) {
    if (!p) return;

    p->state = PROC_READY;

    switch (p->priority) {
        case PRIO_HIGH:
            queue_push(&ready_high, p);
            break;

        case PRIO_MED:
            queue_push(&ready_med, p);
            break;

        case PRIO_LOW:
        default:
            queue_push(&ready_low, p);
            break;
    }
}


// -------------------------------
//   Obtener siguiente proceso
//   (Política: HIGH > MED > LOW)
// -------------------------------
PCB* scheduler_pick_next(void) {
    PCB *p = NULL;

    if (!queue_empty(&ready_high))
        p = queue_pop(&ready_high);
    else if (!queue_empty(&ready_med))
        p = queue_pop(&ready_med);
    else if (!queue_empty(&ready_low))
        p = queue_pop(&ready_low);

    return p;
}


// -------------------------------
//   Obtener proceso actual
// -------------------------------
PCB* scheduler_current(void) {
    return current_process;
}


// -------------------------------
// Cambio de contexto cooperativo
// *Este es REAL pero simple*
// -------------------------------
void scheduler_switch(PCB *next) {
    if (!next) return;
    current_process = next;

    next->state = PROC_RUNNING;

    // Si el proceso nunca ha corrido, entrar a la función
    if (next->ctx.eip != 0 && next->first_run == 0) {
        next->first_run = 1;
        next->entry();  // << Proceso real
        next->state = PROC_ZOMBIE;
        return;
    }

    // Si quieres implementar *context switch real* (con pusha/popa), aquí va.
    // Por ahora es cooperativo y suficiente para el entregable.
}


// -------------------------------
//   Entrada principal del scheduler
//   (lo llama el menú o una syscall)
// -------------------------------
void scheduler_run(void (*print)(const char*)) {
    while (1) {
        PCB *next = scheduler_pick_next();

        if (!next) {
            print("[scheduler] No hay procesos listos.\n");
            return;
        }

        char buf[64];
        snprintf(buf, sizeof(buf), "[scheduler] Ejecutando PID %d\n", next->pid);
        print(buf);

        scheduler_switch(next);
    }
}

// -------------------------------
//   Yield cooperativo
// -------------------------------
void scheduler_yield(void) {
    PCB *current = scheduler_current();
    if (current && current->state == PROC_RUNNING) {
        scheduler_add_ready(current);
    }

    PCB *next = scheduler_pick_next();
    if (next) {
        scheduler_switch(next);
    }
}
