CC ?= i686-elf-gcc
LD ?= i686-elf-ld
AS ?= $(CC)
ASFLAGS := -m32 -x assembler
CFLAGS := -m32 -ffreestanding -nostdlib -fno-pie -fno-pic -Wall -Wextra -Iinclude -Ikernel
LDFLAGS := -m elf_i386 -Ttext 0x100000 -nostdlib -z max-page-size=0x1000 --defsym kmain=kernel_main
QEMU ?= qemu-system-i386

ASM_SRCS := $(wildcard asm/*.asm)
C_SRCS := $(wildcard kernel/*.c)
OBJS := $(ASM_SRCS:.asm=.o) $(C_SRCS:.c=.o)

.PHONY: all run clean

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

asm/%.o: asm/%.asm
	$(AS) $(ASFLAGS) -c $< -o $@

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: kernel.elf
	$(QEMU) -kernel $< -serial stdio

clean:
	rm -f $(OBJS) kernel.elf
