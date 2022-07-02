#ifndef _STDLIB_SHARED_MEMORY_HPP
#define _STDLIB_SHARED_MEMORY_HPP

#include <syscalls>

namespace std {
	inline SMID smMake() {
		return _syscallZero(Syscalls::SM_MAKE);
	}

	inline bool smAllow(SMID smid, PID pid) {
		return _syscallTwo(Syscalls::SM_ALLOW, smid, pid);
	}

	inline bool smRequest(PID pid, SMID smid) {
		return _syscallTwo(Syscalls::SM_REQUEST, pid, smid);
	}

	inline void* smMap(SMID smid) {
		return (void*)_syscallOne(Syscalls::SM_MAP, smid);
	}

	// Abstraction
	namespace sm {
		bool connect(PID pid, SMID smid);
		uint8_t* get(PID pid);
	};
};

#endif
