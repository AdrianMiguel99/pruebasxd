#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* ============================================================
   CONSTANTES
   ============================================================ */

#define IDT_SIZE        256
#define IRQ_BASE        0x20   // PIC remap: IRQ0 → 0x20
#define IRQ_KEYBOARD    (IRQ_BASE + 1)   // 0x21

#define SYSCALL_INT     0x80   // Interrupt para system calls

/* ============================================================
   ESTRUCTURAS IDT
   (deben ser iguales a las usadas en isr.h y idt.asm)
   ============================================================ */

typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

/* ============================================================
   PROTOTIPOS PARA INICIALIZAR LA IDT
   ============================================================ */

/* Inicializa la tabla IDT y configura:
    - IRQ1 teclado → irq_keyboard
    - INT 0x80     → syscall handler
*/
void idt_init(void);

/* Setea una entrada de la IDT */
void idt_set_gate(int n, uint32_t handler_addr);

/* Carga la IDT usando lidt (implementado en ASM) */
void idt_load(uint32_t idt_ptr_addr);

/* ============================================================
   EXTERN DE LOS HANDLERS EN ASM
   ============================================================ */

extern void irq1_keyboard_stub(void);   // irq_keyboard.asm
extern void isr_syscall_stub(void);     // isr_syscall.asm

#endif /* IDT_H */
