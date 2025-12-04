/* ---------------------------------------------------------
 * isr_syscall.asm — Manejador de system calls (INT 0x80)
 * --------------------------------------------------------- */

    .section .text
    .global isr_syscall
    .extern syscall_handler_c   /* función C que procesará los syscalls */

/* ---------------------------------------------------------
 * isr_syscall:
 *  - Guardar todos los registros
 *  - Cambiar a segmento de datos del kernel
 *  - Llamar a syscall_handler_c(eax, ebx, ecx, edx)
 *  - Restaurar contexto
 *  - iret
 * --------------------------------------------------------- */
isr_syscall:
    cli

    /* Guardar contexto completo */
    pushal
    push %ds
    push %es

    /* Cargar descriptores de datos del kernel (0x10) */
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es

    /* Pasar registros a C:
       C ABI usa:
         eax = número syscall
         ebx, ecx, edx = argumentos
    */
    push %edx
    push %ecx
    push %ebx
    push %eax

    call syscall_handler_c

    /* C limpia la pila, no nosotros — así que nada que pop */

    pop %es
    pop %ds
    popal

    sti
    iret
