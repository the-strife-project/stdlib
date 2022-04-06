#ifndef _STDLIB_SYSCALLS_HPP
#define _STDLIB_SYSCALLS_HPP

#define SYSCALL_CLOBBER "cc", "rcx", "r11"

#include <types>
#include <string>

namespace std {
	struct Syscalls {
		enum {
			EXIT,
			MORE_HEAP,
			MMAP,
			MAKE_PROCESS,
			ASLR_GET,
			MAP_IN,
			BACK_FROM_LOADER,
			PUBLISH,
			RESOLVE,
			HALT
		};
	};

	// Generic functions for syscalls
	inline uint64_t _syscallOne(uint64_t id, uint64_t arg1) {
		uint64_t ret;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "D" (id),
					   "S" (arg1)
					 : SYSCALL_CLOBBER);
		return ret;
	}

	inline uint64_t _syscallTwo(uint64_t id, uint64_t arg1, uint64_t arg2) {
		uint64_t ret;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "D" (id),
					   "S" (arg1),
					   "d" (arg2)
					 : SYSCALL_CLOBBER);
		return ret;
	}



	[[noreturn]] inline void exit(size_t val) {
		asm volatile("syscall"
					 :
					 : "D" (Syscalls::EXIT),
					   "S" (val));
		while(true);
	}

	inline uint64_t moreHeap(uint64_t pages=1) {
		return _syscallOne(Syscalls::MORE_HEAP, pages);
	}

	const size_t MMAP_READ  = 1 << 0;
	const size_t MMAP_WRITE = 1 << 1;
	const size_t MMAP_EXEC  = 1 << 2;
	const size_t MMAP_RW  = MMAP_READ | MMAP_WRITE;
	const size_t MMAP_RWX = MMAP_RW | MMAP_EXEC;
	inline void* mmap(size_t npages=1, size_t prot=MMAP_RW) {
		return (void*)_syscallTwo(Syscalls::MMAP, npages, prot);
	}

	bool publish(const std::string& name);
	PID resolve(const std::string& name);

	[[noreturn]] inline void halt() {
		asm volatile("syscall"
					 :
					 : "D" (Syscalls::HALT));
		while(true);
	}
};

#endif
