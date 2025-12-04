#include <stdio.h>

// Implementaciones mínimas para permitir compilar y probar el flujo de alto nivel
volatile char keyboard_buffer[256];
volatile int kb_head = 0;
volatile int kb_tail = 0;

void idt_init(void) {
    puts("[stub] idt_init llamado (sin implementar)");
}

void keyboard_init(void) {
    puts("[stub] keyboard_init llamado (sin implementar)");
}
