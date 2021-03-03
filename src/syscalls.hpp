#ifndef _STDLIB_SYSCALLS_HPP
#define _STDLIB_SYSCALLS_HPP

#include <types>

namespace std {
	struct Syscalls {
		enum {
			EXIT,
			MORE_HEAP
		};
	};

	void exit();

	inline uint64_t moreHeap(uint64_t pages) {
		uint64_t ret;
		asm volatile("syscall" : "=a"(ret) : "D"(Syscalls::MORE_HEAP), "S"(pages));
		return ret;
	}
};

#endif
