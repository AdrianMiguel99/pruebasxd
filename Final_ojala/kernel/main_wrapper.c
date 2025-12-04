#include <stdio.h>

void kernel_main(void);

int main(void) {
    printf("[host] Iniciando kernel_main()...\n");
    kernel_main();
    return 0;
}
