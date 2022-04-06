#ifndef _STDLIB_COMMON_HPP
#define _STDLIB_COMMON_HPP

#include <stdint.h>
#include <stddef.h>

typedef size_t PID;

// Bochs breakpoint for debugging
inline void bochsbrk() { asm volatile("xchgw %bx, %bx"); }

#endif
