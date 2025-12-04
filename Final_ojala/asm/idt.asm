/* ---------------------------------------------------------
 * idt.asm — Rutinas para cargar la IDT
 * --------------------------------------------------------- */

    .section .text
    .global idt_load
    .extern idt_ptr   /* definido en C */

 /* ---------------------------------------------------------
  * idt_load:
  *    - Carga el IDT usando lidt
  *    - idt_ptr es una estructura:
  *        struct {
  *            uint16_t limit;
  *            uint32_t base;
  *        } __attribute__((packed));
  * --------------------------------------------------------- */
idt_load:
    cli
    lidt idt_ptr      /* carga la tabla IDT */
    sti
    ret


/* ---------------------------------------------------------
 * Opcional: funciones auxiliares para habilitar/deshabilitar interrupciones
 * --------------------------------------------------------- */

    .global enable_interrupts
enable_interrupts:
    sti
    ret

    .global disable_interrupts
disable_interrupts:
    cli
    ret
