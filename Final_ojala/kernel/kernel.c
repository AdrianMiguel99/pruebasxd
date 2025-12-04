#include <stdint.h>
#include <stdlib.h>
#ifdef HOST_SIM
#include <stdio.h>
#endif
#include "process.h"
#include "scheduler.h"
#include "queue.h"
#include "syscall.h"

// Declaraciones de inicialización básica
void idt_init(void);
void keyboard_init(void);

// ---------------------------------------------------------
//  VIDEO MODE 0xB8000 — simple print
// ---------------------------------------------------------
#ifdef HOST_SIM
static void flush_stdout(void) {
    fflush(stdout);
}

// En modo simulado, escribe a stdout para evitar accesos a 0xB8000
void kprint(const char *s) {
    fputs(s, stdout);
    flush_stdout();
}

// Para syscall.c (el kernel exporta esta función)
void kprint_sys(const char *s) { kprint(s); }
#else
static char *video = (char*)0xB8000;
static int cursor_pos = 0;

void kprint(const char *s) {
    while (*s) {
        if (*s == '\n') {
            int rest = 160 - (cursor_pos % 160);
            cursor_pos += rest;
            s++;
            continue;
        }
        video[cursor_pos] = *s;
        video[cursor_pos + 1] = 0x07;
        cursor_pos += 2;
        s++;
    }
}

// Para syscall.c (el kernel exporta esta función)
void kprint_sys(const char *s) { kprint(s); }
#endif

// ---------------------------------------------------------
//  KEYBOARD INPUT (IRQ1 usa keyboard_buffer[])
// ---------------------------------------------------------
extern volatile char keyboard_buffer[256];
extern volatile int kb_head, kb_tail;

char getchar_blocking(void) {
#ifdef HOST_SIM
    int c = getchar();
    return (c == EOF) ? '\n' : (char)c;
#else
    while (kb_head == kb_tail) {
        __asm__ volatile("hlt");
    }
    char c = keyboard_buffer[kb_tail];
    kb_tail = (kb_tail + 1) & 0xFF;
    return c;
#endif
}

// leer línea estilo BIOS
int readline(char *buf, int max) {
    int len = 0;

    while (1) {
        char c = getchar_blocking();

        if (c == '\r' || c == '\n') {
            kprint("\n");
            break;
        }

        if (c == '\b') {
            if (len > 0) {
                len--;
                kprint("\b \b");
            }
            continue;
        }

        if (len < max - 1) {
            buf[len++] = c;
            char out[2] = {c, 0};
            kprint(out);
        }
    }

    buf[len] = 0;
    return len;
}

// ---------------------------------------------------------
//  MENÚS
// ---------------------------------------------------------

// ***** SUBMENÚ: Pruebas de procesos *****
void menu_procesos(void) {
    char buf[32];

    while (1) {
        kprint("\n=== MENU PROCESOS ===\n");
        kprint("c) Create process\n");
        kprint("l) List processes\n");
        kprint("k) Kill process\n");
        kprint("r) Run process\n");
        kprint("p) Probar cola prioridad\n");
        kprint("v) Volver\n");
        kprint(">>> ");

        char op = getchar_blocking();
        char tmp[2] = {op, 0};
        kprint(tmp);
        kprint("\n");

        switch (op) {
            case 'c': {
                kprint("Nombre del proceso: ");
                readline(buf, sizeof(buf));
                int pid = process_create(buf, PRIO_MED);
                if (pid < 0) kprint("Error creando proceso\n");
                break;
            }

            case 'l':
                process_list(kprint);
                break;

            case 'k': {
                kprint("PID a matar: ");
                readline(buf, sizeof(buf));
                int pid = atoi(buf);
                process_kill(pid);
                break;
            }

            case 'r': {
                kprint("PID a ejecutar: ");
                readline(buf, sizeof(buf));
                int pid = atoi(buf);
                process_run(pid, kprint);
                break;
            }

            case 'p':
                kprint("Probando scheduler (3 prioridades)...\n");
                scheduler_run(kprint);
                break;

            case 'v':
                return;
        }
    }
}


// ***** MENÚ PRINCIPAL *****
void menu_principal(void) {
    while (1) {
        kprint("\n=== MENU PRINCIPAL ===\n");
        kprint("1) Salir (hlt)\n");
        kprint("2) Pruebas de procesos\n");
        kprint(">>> ");

        char op = getchar_blocking();
        char tmp[2] = {op,0};
        kprint(tmp);
        kprint("\n");

        if (op == '1') {
            kprint("Deteniendo CPU...\n");
#ifdef HOST_SIM
            return;
#else
            while (1) __asm__ volatile("hlt");
#endif
        }

        if (op == '2') {
            menu_procesos();
        }
    }
}


// ---------------------------------------------------------
//  KERNEL MAIN (llamado desde start.asm)
// ---------------------------------------------------------
void kernel_main(void) {
    kprint("Bienvenido al kernel IPv3\n");
    kprint("GRUB ya me puso en modo protegido :)\n");

    // Inicializar subsistemas
    process_init();
    scheduler_init();
    syscall_init();
    idt_init();             // la implementas en idt.asm + idt.c
    keyboard_init();        // la implementas en irq_keyboard.asm + keyboard.c

#ifndef HOST_SIM
    __asm__ volatile("sti");  // habilitar interrupciones
#endif

    menu_principal();
}
