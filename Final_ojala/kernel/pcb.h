#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_PROCESSES 32
#define PROC_NAME_LEN 16

// --------------------------
// Estados del proceso
// --------------------------
typedef enum {
    PROC_UNUSED = 0,   // entrada libre en la tabla
    PROC_NEW,          // creado pero no listo aún
    PROC_READY,        // listo para correr (en cola)
    PROC_RUNNING,      // actualmente ejecutando
    PROC_BLOCKED,      // bloqueado (IPC, I/O, etc.)
    PROC_ZOMBIE        // terminado, pendiente de limpieza
} proc_state_t;

// --------------------------
// Prioridades (Múltiple Cola)
// 0 = más alta
// --------------------------
typedef enum {
    PRIO_HIGH = 0,
    PRIO_MED  = 1,
    PRIO_LOW  = 2
} proc_prio_t;

// --------------------------
// Contexto mínimo de CPU
// (para futuro cambio de contexto real)
// --------------------------
typedef struct cpu_context {
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    // Podrías agregar más registros si luego haces
    // un context switch completo:
    // uint32_t eax, ebx, ecx, edx;
    // uint32_t esi, edi;
    // uint32_t eflags;
} cpu_context_t;

// --------------------------
// PCB: Process Control Block
// --------------------------
typedef struct PCB {
    int          pid;                         // identificador del proceso
    char         name[PROC_NAME_LEN];         // nombre para debug / menú
    proc_state_t state;                       // estado actual
    proc_prio_t  priority;                    // prioridad (para colas)
    cpu_context_t ctx;                        // contexto de CPU
    void       (*entry)(void);                // función de entrada del proceso
    bool         first_run;                   // ¿ya se ejecutó la función entry?

    // Puntero a la pila del proceso (stack propio)
    uint8_t*     stack_base;
    uint32_t     stack_size;
} PCB;

#endif /* PCB_H */
