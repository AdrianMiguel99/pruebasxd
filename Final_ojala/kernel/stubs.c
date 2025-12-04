#include <stdint.h>
#include "syscall.h"

// Buffers de teclado simples para permitir compilación y pruebas básicas
volatile char keyboard_buffer[256];
volatile int kb_head = 0;
volatile int kb_tail = 0;

// Rutinas de inicialización de hardware (stubs por ahora)
void init_idt(void) {}
void pic_remap(void) {}
void idt_init(void) {}
void keyboard_init(void) {}

// Manejadores de interrupciones (stubs)
void keyboard_handler_c(void) {}

// Puente entre la ISR de syscall en ASM y el dispatcher en C
void syscall_handler_c(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx) {
    syscall_dispatch(eax, ebx, ecx, edx);
}
