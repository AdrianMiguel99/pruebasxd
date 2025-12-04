#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

/* ============================================================
   Funciones básicas del sistema
   ============================================================ */

/* Salida por pantalla (modo texto VGA 80x25) */
void print_char(char c);
void print(const char *s);

/* Salida por serial (opcional si quieres agregarla luego) */
void serial_init(void);
void serial_putc(char c);
void serial_puts(const char *s);

/* ============================================================
   Interrupciones y control del CPU
   ============================================================ */

static inline void halt_cpu(void) {
    __asm__ volatile("hlt");
}

static inline void enable_interrupts(void) {
    __asm__ volatile("sti");
}

static inline void disable_interrupts(void) {
    __asm__ volatile("cli");
}

/* ============================================================
   Syscall interface
   ============================================================ */

#define SYSCALL_CREATE_PROC   1
#define SYSCALL_LIST_PROC     2
#define SYSCALL_RUN_PROC      3
#define SYSCALL_KILL_PROC     4
#define SYSCALL_SCHEDULER_TEST  5   /* probar cola prioridad */

/*
   Invocar un system call:
   eax = número de syscall
   ebx = argumento 1
   ecx = argumento 2
   edx = argumento 3
*/
static inline int syscall(int num, uint32_t a1, uint32_t a2, uint32_t a3) {
    int ret;
    __asm__ volatile(
        "int $0x80"
        : "=a"(ret)
        : "a"(num), "b"(a1), "c"(a2), "d"(a3)
    );
    return ret;
}

/* ============================================================
   Memoria (más adelante para paginación)
   ============================================================ */

void memset(void *dst, uint8_t value, uint32_t size);
void memcpy(void *dst, const void *src, uint32_t size);

#endif /* SYSTEM_H */
