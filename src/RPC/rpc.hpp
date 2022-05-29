#ifndef _STDLIB_RPC_HPP
#define _STDLIB_RPC_HPP

#include <syscalls>

namespace std {
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
