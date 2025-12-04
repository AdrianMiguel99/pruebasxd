#include "pcb.h"
#include "scheduler.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PROCESS_STACK_SIZE 4096

// Tabla global de procesos
static PCB proc_table[MAX_PROCESSES];
static int next_pid = 1;

static void default_entry(void) {
    // Proceso de prueba que simplemente retorna
}

// ---------------------------
//  Inicialización global
// ---------------------------
void process_init(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        proc_table[i].state = PROC_UNUSED;
    }
}


// ---------------------------
//  Buscar entrada libre
// ---------------------------
static int find_free_slot(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (proc_table[i].state == PROC_UNUSED ||
            proc_table[i].state == PROC_ZOMBIE) {
            return i;
        }
    }
    return -1;
}


// ---------------------------
//  Crear proceso real
// ---------------------------
int process_create(const char *name, proc_prio_t prio) {
    int slot = find_free_slot();
    if (slot < 0) return -1;

    PCB *p = &proc_table[slot];

    // PID
    p->pid = next_pid++;

    // Nombre
    strncpy(p->name, name, PROC_NAME_LEN - 1);
    p->name[PROC_NAME_LEN - 1] = 0;

    // Prioridad
    p->priority = prio;

    // Estado inicial
    p->state = PROC_NEW;

    // Función del proceso
    p->entry = default_entry;
    p->first_run = false;

    // Crear pila del proceso
    p->stack_size = PROCESS_STACK_SIZE;
    p->stack_base = (uint8_t*) malloc(PROCESS_STACK_SIZE);

    if (!p->stack_base) {
        p->state = PROC_UNUSED;
        return -1;
    }

    // Configurar contexto inicial
    uint32_t stack_top = (uint32_t)(p->stack_base + p->stack_size);

    p->ctx.esp = stack_top;
    p->ctx.ebp = stack_top;
    p->ctx.eip = (uint32_t) p->entry;

    // Proceso listo para scheduler
    p->state = PROC_READY;
    scheduler_add_ready(p);

    return p->pid;
}


// ---------------------------
//  Listar procesos
// ---------------------------
void process_list(void (*print)(const char*)) {
    char buf[64];

    print("\nPID   STATE       PRIO   NAME\n");

    for (int i = 0; i < MAX_PROCESSES; i++) {
        PCB *p = &proc_table[i];
        if (p->state == PROC_UNUSED)
            continue;

        const char *state = "UNK";
        switch (p->state) {
            case PROC_NEW:     state = "NEW"; break;
            case PROC_READY:   state = "READY"; break;
            case PROC_RUNNING: state = "RUNNING"; break;
            case PROC_BLOCKED: state = "BLOCKED"; break;
            case PROC_ZOMBIE:  state = "ZOMBIE"; break;
            default: break;
        }

        const char *prio = (p->priority == PRIO_HIGH) ? "HIGH" :
                           (p->priority == PRIO_MED)  ? "MED " :
                                                        "LOW ";

        snprintf(buf, sizeof(buf),
                 "%d     %-10s %-5s  %s\n",
                 p->pid, state, prio, p->name);
        print(buf);
    }
}


// ---------------------------
//  Matar proceso
// ---------------------------
int process_kill(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        PCB *p = &proc_table[i];
        if (p->pid == pid && p->state != PROC_UNUSED) {
            p->state = PROC_ZOMBIE;
            return 0;
        }
    }
    return -1;
}


// ---------------------------
//  Ejecutar proceso (síncrono)
//  *esto es real pero simple*
// ---------------------------
int process_run(int pid, void (*print)(const char*)) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        PCB *p = &proc_table[i];

        if (p->pid == pid && p->state == PROC_READY) {
            p->state = PROC_RUNNING;

            print("\n[OS] Running process: ");
            print(p->name);
            print("\n");

            // Llamada directa (simula context switch cooperativo)
            if (p->entry)
                p->entry();

            print("[OS] Process finished: ");
            print(p->name);
            print("\n");

            p->state = PROC_ZOMBIE;
            return 0;
        }
    }
    return -1;
}


// ---------------------------
//  Buscar PCB por PID (útil para scheduler e IPC)
// ---------------------------
PCB* process_get(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (proc_table[i].pid == pid &&
            proc_table[i].state != PROC_UNUSED) {
            return &proc_table[i];
        }
    }
    return 0;
}
