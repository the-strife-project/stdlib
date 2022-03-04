#ifndef _STDLIB_SYSCALLS_HPP
#define _STDLIB_SYSCALLS_HPP

#define SYSCALL_CLOBBER "cc", "rcx", "r11"

#include <types>

namespace std {
	struct Syscalls {
		enum {
			EXIT,
			MORE_HEAP
		};
	};

	inline void exit() {
		asm volatile("syscall"
					 :: "D"(Syscalls::EXIT)
					 : SYSCALL_CLOBBER);
	}

	inline uint64_t moreHeap(uint64_t pages=1) {
		uint64_t ret;
		asm volatile("syscall"
					 : "=a"(ret)
					 : "D"(Syscalls::MORE_HEAP), "S"(pages)
					 : SYSCALL_CLOBBER);
		return ret;
	}

	int test(); // Test function (for relocation)
};

#endif
