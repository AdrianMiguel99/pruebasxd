#include <stdint.h>

// IDT pointer structure expected by asm/idt.asm
struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr = {0, 0};

// -----------------------------------------------------------
// Hardware/PIC stubs (suficiente para un arranque mínimo)
// -----------------------------------------------------------
void pic_remap(void) {}
void init_idt(void) {}
void pic_send_eoi(void) {}
void keyboard_handler_c(void) {}
uint32_t syscall_handler_c(uint32_t id, uint32_t a1, uint32_t a2, uint32_t a3) {
    (void)id; (void)a1; (void)a2; (void)a3;
    return 0;
}

// -----------------------------------------------------------
// Salida en modo texto VGA 80x25
// -----------------------------------------------------------
static volatile uint16_t *const vram = (uint16_t*)0xB8000;
static uint32_t cursor = 0;

static void vga_putc(char c) {
    if (c == '\n') {
        cursor = (cursor / 80 + 1) * 80;
        return;
    }

    vram[cursor++] = (uint16_t)c | (0x07 << 8);
}

static void vga_puts(const char *s) {
    while (*s) {
        vga_putc(*s++);
    }
}

// -----------------------------------------------------------
// Punto de entrada llamado por asm/start.asm
// -----------------------------------------------------------
void kmain(void) {
    vga_puts("Boot minimal del kernel...\n");
    vga_puts("Listo para iniciar en QEMU.\n");

    // Bucle de espera
    while (1) {
        __asm__ volatile("hlt");
    }
}
