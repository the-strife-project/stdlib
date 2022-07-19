#ifndef _STDLIB_COMMON_HPP
#define _STDLIB_COMMON_HPP

#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096
#define NULL_PID 0

#define PAGE(x) ((x) & ~0xFFF)
#define PAGEOFF(x) ((x) & 0xFFF)
#define NPAGES(x) (((x) + PAGE_SIZE - 1) / PAGE_SIZE)

namespace std {
	typedef unsigned short PID;
	typedef size_t RPID;
	typedef size_t SMID;

	struct UUID {
		uint64_t a, b;

		UUID() = default;
		UUID(uint64_t a, uint64_t b) : a(a), b(b) {}
	};
}

// Bochs breakpoint for debugging
inline void bochsbrk() { asm volatile("xchgw %bx, %bx"); }

#define IGNORE(x) ((void)x)
#define HALT_AND_CATCH_FIRE() (*(uint64_t*)0 = 0)
#define fallthrough __attribute__((__fallthrough__))

#endif
