#ifndef _STDLIB_SYSCALLS_HPP
#define _STDLIB_SYSCALLS_HPP

#define SYSCALL_CLOBBER "cc", "rcx", "r11"

#include <types>

namespace std {
	struct Syscalls {
		enum {
			EXIT,
			MORE_HEAP,
			MMAP,
			MAKE_PROCESS,
			ASLR_GET,
			MAP_IN,
			BACK_FROM_LOADER
		};
	};

	inline void exit(size_t val) {
		asm volatile("syscall"
					 :
					 : "D" (Syscalls::EXIT),
					   "S" (val)
					 : SYSCALL_CLOBBER);
	}

	inline uint64_t moreHeap(uint64_t pages=1) {
		uint64_t ret;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "D" (Syscalls::MORE_HEAP),
					   "S"(pages)
					 : SYSCALL_CLOBBER);
		return ret;
	}

	const size_t MMAP_READ  = 1 << 0;
	const size_t MMAP_WRITE = 1 << 1;
	const size_t MMAP_EXEC  = 1 << 2;
	const size_t MMAP_RW  = MMAP_READ | MMAP_WRITE;
	const size_t MMAP_RWX = MMAP_RW | MMAP_EXEC;
	inline void* mmap(size_t npages=1, size_t prot=MMAP_RW) {
		void* ret;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "D" (Syscalls::MMAP),
					   "S" (npages),
					   "d" (prot)
					 : SYSCALL_CLOBBER);
		return ret;
	}

	int test(); // Test function (for relocation)
};

#endif
