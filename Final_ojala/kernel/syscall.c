#include <stdint.h>
#include "syscall.h"
#include "process.h"
#include "scheduler.h"

// ======================================================
//  IDs de system calls
// ======================================================
#define SYSCALL_PRINT     1
#define SYSCALL_CREATE    2
#define SYSCALL_LIST      3
#define SYSCALL_KILL      4
#define SYSCALL_RUN       5
#define SYSCALL_YIELD     6

// ======================================================
//  Prototipos de handlers (implementados aquí)
// ======================================================
static uint32_t sys_print(uintptr_t msg_ptr, uintptr_t _arg2, uintptr_t _arg3);
static uint32_t sys_create(uintptr_t name_ptr, uintptr_t _arg2, uintptr_t _arg3);
static uint32_t sys_list(uintptr_t _a1, uintptr_t _a2, uintptr_t _a3);
static uint32_t sys_kill(uintptr_t pid, uintptr_t _arg2, uintptr_t _arg3);
static uint32_t sys_run(uintptr_t pid, uintptr_t _arg2, uintptr_t _arg3);
static uint32_t sys_yield(uintptr_t _a1, uintptr_t _a2, uintptr_t _a3);

// Esta función es exportada por kernel.c
extern void kprint_sys(const char *msg);

// ======================================================
//  TABLA DE SYSCALLS
// ======================================================
typedef uint32_t (*syscall_fn_t)(uintptr_t, uintptr_t, uintptr_t);

static syscall_fn_t syscall_table[] = {
    0,
    sys_print,    // 1
    sys_create,   // 2
    sys_list,     // 3
    sys_kill,     // 4
    sys_run,      // 5
    sys_yield     // 6
};

#define SYSCALL_COUNT (sizeof(syscall_table)/sizeof(syscall_fn_t))

// ======================================================
//  Inicialización
// ======================================================
void syscall_init(void) {
    // Nada especial por ahora
    // La IDT ya apunta a la ISR 0x80 que terminará llamando a syscall_dispatch()
}

// ======================================================
//  DISPATCHER — llamado desde ASM (isr_syscall.asm)
// ======================================================
uint32_t syscall_dispatch(uint32_t id, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {

    if (id >= SYSCALL_COUNT || syscall_table[id] == 0) {
        kprint_sys("SYSCALL INVALIDA\n");
        return (uint32_t)-1;
    }

    return syscall_table[id](arg1, arg2, arg3);
}

uint32_t syscall_handler_c(uint32_t id, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    return syscall_dispatch(id, arg1, arg2, arg3);
}


// ======================================================
//  IMPLEMENTACIÓN DE CADA SYSCALL
// ======================================================

static uint32_t sys_print(uintptr_t msg_ptr, uintptr_t _arg2, uintptr_t _arg3) {
    (void)_arg2; (void)_arg3;
    const char *msg = (const char*)msg_ptr;
    kprint_sys(msg);
    return 0;
}

static uint32_t sys_create(uintptr_t name_ptr, uintptr_t _arg2, uintptr_t _arg3) {
    (void)_arg2; (void)_arg3;
    const char *name = (const char*)name_ptr;
    return process_create(name, PRIO_MED);
}

static uint32_t sys_list(uintptr_t _a1, uintptr_t _a2, uintptr_t _a3) {
    (void)_a1; (void)_a2; (void)_a3;
    process_list(kprint_sys);
    return 0;
}

static uint32_t sys_kill(uintptr_t pid, uintptr_t _arg2, uintptr_t _arg3) {
    (void)_arg2; (void)_arg3;
    return process_kill(pid);
}

static uint32_t sys_run(uintptr_t pid, uintptr_t _arg2, uintptr_t _arg3) {
    (void)_arg2; (void)_arg3;
    return process_run(pid, kprint_sys);
}

static uint32_t sys_yield(uintptr_t _a1, uintptr_t _a2, uintptr_t _a3) {
    (void)_a1; (void)_a2; (void)_a3;
    scheduler_yield();
    return 0;
}


// ======================================================
//  WRAPPERS PARA LLAMAR A `int 0x80` DESDE EL KERNEL
// ======================================================
uint32_t syscall_print(const char *msg) {
    return syscall_dispatch(SYSCALL_PRINT, (uint32_t)(uintptr_t)msg, 0, 0);
}

uint32_t syscall_create(const char *name) {
    return syscall_dispatch(SYSCALL_CREATE, (uint32_t)(uintptr_t)name, 0, 0);
}

uint32_t syscall_list(void) {
    return syscall_dispatch(SYSCALL_LIST, 0, 0, 0);
}

uint32_t syscall_kill(uint32_t pid) {
    return syscall_dispatch(SYSCALL_KILL, pid, 0, 0);
}

uint32_t syscall_run(uint32_t pid) {
    return syscall_dispatch(SYSCALL_RUN, pid, 0, 0);
}

uint32_t syscall_yield(void) {
    return syscall_dispatch(SYSCALL_YIELD, 0, 0, 0);
}
