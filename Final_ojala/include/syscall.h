#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

void syscall_init(void);
uint32_t syscall_dispatch(uint32_t id, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);
uint32_t syscall_handler_c(uint32_t id, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);

uint32_t syscall_print(const char *msg);
uint32_t syscall_create(const char *name);
uint32_t syscall_list(void);
uint32_t syscall_kill(uint32_t pid);
uint32_t syscall_run(uint32_t pid);
uint32_t syscall_yield(void);

#endif /* SYSCALL_H */
