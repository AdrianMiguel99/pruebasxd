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
static uint32_t sys_print(const char *msg);
static uint32_t sys_create(const char *name);
static uint32_t sys_list(void);
static uint32_t sys_kill(uint32_t pid);
static uint32_t sys_run(uint32_t pid);
static uint32_t sys_yield(void);

// Esta función es exportada por kernel.c
extern void kprint_sys(const char *msg);

// ======================================================
//  TABLA DE SYSCALLS
// ======================================================
typedef uint32_t (*syscall_fn_t)(uint32_t, uint32_t, uint32_t);

static syscall_fn_t syscall_table[] = {
    0,
    (syscall_fn_t) sys_print,    // 1
    (syscall_fn_t) sys_create,   // 2
    (syscall_fn_t) sys_list,     // 3
    (syscall_fn_t) sys_kill,     // 4
    (syscall_fn_t) sys_run,      // 5
    (syscall_fn_t) sys_yield     // 6
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
uint32_t syscall_dispatch(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3) {

    if (id >= SYSCALL_COUNT || syscall_table[id] == 0) {
        kprint_sys("SYSCALL INVALIDA\n");
        return (uint32_t)-1;
    }

    return syscall_table[id](arg1, arg2, arg3);
}


// ======================================================
//  IMPLEMENTACIÓN DE CADA SYSCALL
// ======================================================

static uint32_t sys_print(const char *msg) {
    kprint_sys(msg);
    return 0;
}

static uint32_t sys_create(const char *name) {
    return process_create(name, PRIO_MED);
}

static uint32_t sys_list(void) {
    process_list(kprint_sys);
    return 0;
}

static uint32_t sys_kill(uint32_t pid) {
    return process_kill(pid);
}

static uint32_t sys_run(uint32_t pid) {
    return process_run(pid, kprint_sys);
}

static uint32_t sys_yield(void) {
    scheduler_yield();
    return 0;
}


// ======================================================
//  WRAPPERS PARA LLAMAR A `int 0x80` DESDE EL KERNEL
// ======================================================
uint32_t syscall_print(const char *msg) {
    uint32_t ret;
    __asm__ volatile (
        "movl %1, %%eax\n"
        "movl %2, %%ebx\n"
        "int $0x80\n"
        "movl %%eax, %0\n"
        : "=r"(ret)
        : "r"(SYSCALL_PRINT), "r"(msg)
        : "%eax", "%ebx"
    );
    return ret;
}

uint32_t syscall_create(const char *name) {
    uint32_t ret;
    __asm__ volatile (
        "movl %1, %%eax\n"
        "movl %2, %%ebx\n"
        "int $0x80\n"
        "movl %%eax, %0\n"
        : "=r"(ret)
        : "r"(SYSCALL_CREATE), "r"(name)
        : "%eax", "%ebx"
    );
    return ret;
}

uint32_t syscall_list(void) {
    uint32_t ret;
    __asm__ volatile (
        "movl %1, %%eax\n"
        "int $0x80\n"
        "movl %%eax, %0\n"
        : "=r"(ret)
        : "r"(SYSCALL_LIST)
        : "%eax"
    );
    return ret;
}

uint32_t syscall_kill(uint32_t pid) {
    uint32_t ret;
    __asm__ volatile (
        "movl %1, %%eax\n"
        "movl %2, %%ebx\n"
        "int $0x80\n"
        "movl %%eax, %0\n"
        : "=r"(ret)
        : "r"(SYSCALL_KILL), "r"(pid)
        : "%eax", "%ebx"
    );
    return ret;
}

uint32_t syscall_run(uint32_t pid) {
    uint32_t ret;
    __asm__ volatile (
        "movl %1, %%eax\n"
        "movl %2, %%ebx\n"
        "int $0x80\n"
        "movl %%eax, %0\n"
        : "=r"(ret)
        : "r"(SYSCALL_RUN), "r"(pid)
        : "%eax", "%ebx"
    );
    return ret;
}

uint32_t syscall_yield(void) {
    uint32_t ret;
    __asm__ volatile (
        "movl %1, %%eax\n"
        "int $0x80\n"
        "movl %%eax, %0\n"
        : "=r"(ret)
        : "r"(SYSCALL_YIELD)
        : "%eax"
    );
    return ret;
}
