#ifndef PCB_H
#define PCB_H

#include <stdint.h>

/* ============================================================
   ESTADOS DEL PROCESO
   ============================================================ */

typedef enum {
    PROC_UNUSED = 0,
    PROC_READY,
    PROC_RUNNING,
    PROC_WAITING,
    PROC_ZOMBIE
} proc_state_t;

/* ============================================================
   PRIORIDADES (COLA MULTINIVEL)
   0 = Alta  | 1 = Media | 2 = Baja
   ============================================================ */

typedef enum {
    PRIO_HIGH = 0,
    PRIO_MEDIUM = 1,
    PRIO_LOW = 2
} proc_priority_t;

/* ============================================================
   PCB — Process Control Block
   ============================================================ */

typedef struct pcb {
    uint32_t pid;
    char name[16];

    proc_state_t state;
    proc_priority_t priority;

    /* Dirección de ejecución del proceso */
    void (*entry)(struct pcb *);

    /* Siguiente nodo en la cola de scheduler */
    struct pcb *next;
} pcb_t;

/* ============================================================
   TABLA GLOBAL DE PROCESOS
   ============================================================ */

#define MAX_PROCS 32

extern pcb_t pcb_table[MAX_PROCS];
extern uint32_t next_pid;

/* ============================================================
   API DE MANEJO DE PROCESOS (process.c)
   ============================================================ */

/* Inicializar tabla de PCBs */
void process_init(void);

/* Crear un proceso */
int process_create(const char *name, proc_priority_t prio);

/* Listar procesos */
void process_list(void);

/* Ejecutar un proceso (sin scheduler todavía) */
int process_run(uint32_t pid);

/* Matar un proceso */
int process_kill(uint32_t pid);

#endif /* PCB_H */
