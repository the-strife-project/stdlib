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
			MUNMAP,
			// Loader only
			MAKE_PROCESS,
			ASLR_GET,
			MAP_IN,
			BACK_FROM_LOADER,
			// PSNS
			FIND_PSNS,
			// RPC
			HALT,
			RPC,
			ENABLE_RPC,
			RPC_MORE_STACKS, // Kernel only
			// Shared memory
			SM_MAKE,
			SM_ALLOW,
			SM_REQUEST,
			SM_GETSIZE,
			SM_DROP,
			SM_MAP,
			// Special permissions
			ALLOW_IO,
			ALLOW_PHYS,
			GET_PHYS,
			MAP_PHYS,
			// Task-related
			GET_PID,
			GET_ORIG_PID,
			EXEC,
			GET_LAST_LOADER_ERROR,
			GET_KILL_REASON,
			GET_EXIT_VALUE,
			WAIT,
			// Locks
			LOCK,
			WAKE,
		};
	};

	// From this point onwards, this file gets very little interesting



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

	inline uint64_t _syscallFour(uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
		uint64_t ret;
		register size_t r10 asm("r10") = arg3;
		register size_t r8 asm("r8") = arg4;
		asm volatile("syscall"
					 : "=a" (ret)
					 : "D" (id),
					   "S" (arg1),
					   "d" (arg2),
					   "r" (r10),
					   "r" (r8)
					 : SYSCALL_CLOBBER);
		return ret;
	}



	// Beginning actual syscalls
	[[noreturn]] inline void exit(size_t val=0) {
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

	inline void munmap(void* ptr, size_t npages=1) {
		_syscallTwo(Syscalls::MUNMAP, (uint64_t)ptr, npages);
	}

	bool publish(const std::string& name);
	PID resolve(const std::string& name);

	[[noreturn]] inline void halt() {
		asm volatile("syscall"
					 :
					 : "D" (Syscalls::HALT));
		while(true);
	}



	// --- RPC SERVER ---
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



	// --- SPECIAL PERMISSIONS ---
	inline size_t allowIO() { return _syscallZero(Syscalls::ALLOW_IO); }
	inline size_t allowPhys() { return _syscallZero(Syscalls::ALLOW_PHYS); }

	inline uint64_t getPhys(uint64_t x) {
		return _syscallOne(Syscalls::GET_PHYS, x);
	}

	const bool MAP_PHYS_WRITE = true;
	const bool MAP_PHYS_RO = false;
	const bool MAP_PHYS_DONT_CACHE = true;
	const bool MAP_PHYS_CACHE = false;
	inline size_t mapPhys(size_t phys, size_t npages=1, bool write=true, size_t noCache=false) {
		size_t prot = 0;
		if(write) prot |= 0b1;
		if(noCache) prot |= 0b10;
		return _syscallThree(Syscalls::MAP_PHYS, phys, npages, prot);
	}



	// --- TASK-RELATED ---
	inline PID getPID() { return _syscallZero(Syscalls::GET_PID); }
	inline PID getOrigPID() { return _syscallZero(Syscalls::GET_ORIG_PID); }
	// The underscore is not to confuse it with run() by mistake
	inline PID _exec(void* buffer, size_t sz, void* runtime, size_t rtsz) {
		return _syscallFour(Syscalls::EXEC,
							(uint64_t)buffer,
							sz,
							(uint64_t)runtime,
							rtsz);
	}
	inline size_t getLastLoaderError() {
		return _syscallZero(Syscalls::GET_LAST_LOADER_ERROR);
	}
	inline size_t getKillReason(PID pid) {
		return _syscallOne(Syscalls::GET_KILL_REASON, pid);
	}
	inline size_t getExitValue(PID pid) {
		return _syscallOne(Syscalls::GET_EXIT_VALUE, pid);
	}
	inline void wait(PID pid) {
		_syscallOne(Syscalls::WAIT, pid);
	}



	// --- LOCKS ---
	inline void lockme() { _syscallZero(Syscalls::LOCK); }
	inline bool wake(std::PID pid) { return _syscallOne(Syscalls::WAKE, pid); }
};

#endif
