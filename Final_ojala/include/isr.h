#ifndef ISR_H
#define ISR_H

#include <stdint.h>

/* ============================================================
   ISR & IRQ HANDLERS (prototipos para C)
   ============================================================ */

/* Llamado desde irq_keyboard.asm (IRQ1) */
void keyboard_handler_c(void);

/* Llamado desde isr_syscall.asm (INT 0x80) */
void syscall_handler_c(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx);

/* Para enviar End-of-Interrupt al PIC (implementado en C o ASM) */
void pic_send_eoi(void);

/* ============================================================
   IDT ENTRIES
   ============================================================ */

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* ============================================================
   FUNCIONES DE INICIALIZACIÓN (definidas en idt.asm)
   ============================================================ */

/* Carga la IDT */
void idt_load(void);

/* Setea una puerta (interrupt gate) */
void idt_set_gate(int n, uint32_t handler);

/* Inicializa completamente la IDT (requiere idt.asm + C) */
void idt_init(void);

#endif /* ISR_H */
