#ifndef _STDLIB_SYSCALLS_HPP
#define _STDLIB_SYSCALLS_HPP

#define SYSCALL_CLOBBER "cc", "rcx", "r11"

#include <types>
#include <string>
#include <export>

namespace std {
	struct Syscalls {
		enum {
			EXIT,
			MORE_HEAP,
			MMAP,
			// Loader only
			MAKE_PROCESS,
			ASLR_GET,
			MAP_IN,
			BACK_FROM_LOADER,
			// PSNS
			PUBLISH,
			RESOLVE,
			// RPC
			HALT,
			RPC,
			ENABLE_RPC,
			RPC_MORE_STACKS, // Kernel only
			// Shared memory
			SM_MAKE,
			SM_ALLOW,
			SM_REQUEST,
			SM_MAP
		};
	};

	// Generic functions for syscalls
	inline uint64_t _syscallZero(uint64_t id) {
		uint64_t ret;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "D" (id)
					 : SYSCALL_CLOBBER);
		return ret;
	}

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

	inline uint64_t _syscallThree(uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
		uint64_t ret;
		register size_t r10 asm("r10") = arg3;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "D" (id),
					   "S" (arg1),
					   "d" (arg2),
					   "r" (r10)
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

	// --- RPC ---
	// Server
	uint64_t rpcHandler(PID client, RPID id, uint64_t arg0, uint64_t arg1,
						uint64_t arg2, uint64_t arg3);
	// No std::exportProcedure calls after enabling RPC!
	extern "C" void rpcEntry();
	inline void enableRPC(void (*entry)()=rpcEntry) {
		asm volatile("syscall"
					 :
					 : "D" (Syscalls::ENABLE_RPC),
					   "S" (entry)
					 : SYSCALL_CLOBBER);
	}
};

#endif
