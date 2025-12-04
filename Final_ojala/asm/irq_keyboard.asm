/* ---------------------------------------------------------
 * irq_keyboard.asm — Manejador de interrupción de teclado (IRQ1)
 * Vector = 0x21 (luego del remapeo del PIC)
 * --------------------------------------------------------- */

    .section .text
    .global irq1_stub
    .extern keyboard_handler_c
    .extern pic_send_eoi

/* ---------------------------------------------------------
 * irq1_stub:
 *  - Se ejecuta cuando llega IRQ1 (teclado)
 *  - Guarda todos los registros
 *  - Cambia segmentos a kernel DS (0x10)
 *  - Llama a keyboard_handler_c() en C
 *  - Envia EOI al PIC
 *  - iret
 * --------------------------------------------------------- */
irq1_stub:
    cli

    pushal
    push %ds
    push %es

    /* Cargar segmentos de datos del kernel */
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es

    /* Llamar manejador en C */
    call keyboard_handler_c

    /* Finalizar interrupción en el PIC */
    call pic_send_eoi

    /* Restaurar segmentos y registros */
    pop %es
    pop %ds
    popal

    sti
    iret
