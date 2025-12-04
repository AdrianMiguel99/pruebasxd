#ifndef PCB_H
#define PCB_H

#include <stdint.h>

#define MAX_PROCESSES 32
#define PROC_NAME_LEN 16

// Estados del proceso
typedef enum {
    PROC_UNUSED = 0,
    PROC_NEW,
    PROC_READY,
    PROC_RUNNING,
    PROC_BLOCKED,
    PROC_ZOMBIE
} proc_state_t;

// Prioridades (0 = alta)
typedef enum {
    PRIO_HIGH = 0,
    PRIO_MED  = 1,
    PRIO_LOW  = 2
} proc_prio_t;

// Contexto mínimo de CPU (placeholder para futuros switches de contexto)
typedef struct cpu_context {
    uintptr_t esp;
    uintptr_t ebp;
    uintptr_t eip;
} cpu_context_t;

// PCB: Process Control Block
typedef struct PCB {
    int           pid;
    char          name[PROC_NAME_LEN];
    proc_state_t  state;
    proc_prio_t   priority;
    cpu_context_t ctx;
    void        (*entry)(void);
    uint8_t      *stack_base;
    uint32_t      stack_size;
    uint8_t       first_run;
} PCB;

#endif /* PCB_H */
