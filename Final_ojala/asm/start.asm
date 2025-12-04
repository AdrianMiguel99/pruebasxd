/* ---------------------------------------------------------
 *  start.S - Entrada del kernel para GRUB (Multiboot v1)
 * --------------------------------------------------------- */

    .section .multiboot
    .align 4
    .global multiboot_header
multiboot_header:
    .long 0x1BADB002            /* multiboot magic */
    .long 0x00010003            /* flags: align + memory info */
    .long -(0x1BADB002 + 0x00010003)

    .text
    .global _start
    .extern kmain
    .extern idt_load
    .extern init_idt
    .extern pic_remap

_start:
    cli

    /* cargar segmento de datos del kernel (GRUB usa 0x10 para data) */
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss

    /* configurar stack */
    movl $stack_top, %esp

    /* Inicializar PIC + IDT */
    call pic_remap
    call init_idt
    call idt_load

    sti

    /* entrar al kernel en C */
    call kmain

.hang:
    cli
    hlt
    jmp .hang


/* ---------------------------------------------------------
 *  Stack (4 KB)
 * --------------------------------------------------------- */
    .section .bss
    .align 16
stack_bottom:
    .skip 4096
stack_top:
