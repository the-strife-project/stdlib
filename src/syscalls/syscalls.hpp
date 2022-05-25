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
			MAKE_PROCESS,
			ASLR_GET,
			MAP_IN,
			BACK_FROM_LOADER,
			PUBLISH,
			RESOLVE,
			HALT,
			RPC,
			ENABLE_RPC,
			RPC_MORE_STACKS
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
	// Client
	// How many arguments though 🤔
	// It's better to create a function for each possibility, so GCC
	//   can handle registers better.
	// Zero args
	inline uint64_t rpc(PID remote, RPID id) {
		uint64_t ret;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "a" (remote),
					   "D" (Syscalls::RPC),
					   "S" (id)
					 : SYSCALL_CLOBBER);
		return ret;
	}
	// One argument
	inline uint64_t rpc(PID remote, RPID id, size_t a) {
		uint64_t ret;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "a" (remote),
					   "D" (Syscalls::RPC),
					   "S" (id),
					   "d" (a)
					 : SYSCALL_CLOBBER);
		return ret;
	}
	// Two arguments
	inline uint64_t rpc(PID remote, RPID id, size_t a, size_t b) {
		uint64_t ret;
		register size_t r10 asm("r10") = b;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "a" (remote),
					   "D" (Syscalls::RPC),
					   "S" (id),
					   "d" (a),
					   "r" (r10)
					 : SYSCALL_CLOBBER);
		return ret;
	}
	// Three arguments
	inline uint64_t rpc(PID remote, RPID id, size_t a, size_t b, size_t c) {
		uint64_t ret;
		register size_t r10 asm("r10") = b;
		register size_t r8 asm("r8") = c;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "a" (remote),
					   "D" (Syscalls::RPC),
					   "S" (id),
					   "d" (a),
					   "r" (r10),
					   "r" (r8)
					 : SYSCALL_CLOBBER);
		return ret;
	}
	// Four arguments
	inline uint64_t rpc(PID remote, RPID id, size_t a, size_t b, size_t c, size_t d) {
		uint64_t ret;
		register size_t r10 asm("r10") = b;
		register size_t r8 asm("r8") = c;
		register size_t r9 asm("r9") = d;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "a" (remote),
					   "D" (Syscalls::RPC),
					   "S" (id),
					   "d" (a),
					   "r" (r10),
					   "r" (r8),
					   "r" (r9)
					 : SYSCALL_CLOBBER);
		return ret;
	}
};

#endif
