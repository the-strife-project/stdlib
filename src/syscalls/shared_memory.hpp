#ifndef _STDLIB_SHARED_MEMORY_HPP
#define _STDLIB_SHARED_MEMORY_HPP

#include <syscalls>
#include <pair>

namespace std {
	inline SMID smMake(size_t npages=1) {
		return _syscallOne(Syscalls::SM_MAKE, npages);
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

	inline size_t smGetSize(SMID smid) {
		return _syscallOne(Syscalls::SM_GETSIZE, smid);
	}

	inline void smDrop(SMID smid) {
		_syscallOne(Syscalls::SM_DROP, smid);
	}

	// Abstraction
	namespace sm {
		bool connect(PID pid, SMID smid);
		uint8_t* get(PID pid);
		size_t getPages(PID pid);

		pair<uint8_t*, size_t> link(PID pid, SMID smid);
		void unlink(SMID smid);
	};
};

#endif
