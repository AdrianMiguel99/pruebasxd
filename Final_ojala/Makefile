CC = gcc
LD = ld
MKRESCUE = grub-mkrescue
QEMU = qemu-system-i386

HOST_CFLAGS = -std=c11 -Wall -Wextra -g -Iinclude -Ikernel -DHOST_SIM
BARE_CFLAGS = -std=gnu11 -ffreestanding -fno-pic -fno-pie -m32 -Wall -Wextra -Iinclude -Ikernel
BARE_LDFLAGS = -m elf_i386 -T link.ld -nostdlib

HOST_SRC := $(filter-out kernel/bare_kernel.c,$(wildcard kernel/*.c))
HOST_OBJ := $(HOST_SRC:.c=.o)
HOST_TARGET := kernel_demo

BUILD_DIR := build
BARE_OBJ := $(BUILD_DIR)/start.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/irq_keyboard.o $(BUILD_DIR)/isr_syscall.o $(BUILD_DIR)/bare_kernel.o
ISO := $(BUILD_DIR)/kernel.iso
KERNEL_ELF := $(BUILD_DIR)/kernel.elf

.PHONY: all host iso run clean

all: host

host: $(HOST_TARGET)

$(HOST_TARGET): $(HOST_OBJ)
	$(CC) $(HOST_OBJ) -o $@

%.o: %.c
	$(CC) $(HOST_CFLAGS) -c -o $@ $<

# ------------------------------------------------------------
#    Bare-metal build (Multiboot + QEMU)
# ------------------------------------------------------------
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: asm/%.asm | $(BUILD_DIR)
	$(CC) -m32 -x assembler-with-cpp -c -o $@ $<

$(BUILD_DIR)/bare_kernel.o: kernel/bare_kernel.c | $(BUILD_DIR)
	$(CC) $(BARE_CFLAGS) -c -o $@ $<

$(KERNEL_ELF): $(BARE_OBJ) link.ld | $(BUILD_DIR)
	$(LD) $(BARE_LDFLAGS) -o $@ $(BARE_OBJ)

$(ISO): $(KERNEL_ELF) grub.cfg | $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/iso/boot/grub
	cp $(KERNEL_ELF) $(BUILD_DIR)/iso/boot/kernel.elf
	cp grub.cfg $(BUILD_DIR)/iso/boot/grub/grub.cfg
	$(MKRESCUE) -o $@ $(BUILD_DIR)/iso

iso: $(ISO)

run: $(ISO)
	$(QEMU) -cdrom $(ISO)

clean:
	rm -f $(HOST_OBJ) $(HOST_TARGET)
	rm -rf $(BUILD_DIR)
