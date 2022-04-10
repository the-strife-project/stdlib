#ifndef _STDLIB_KKILL_HPP
#define _STDLIB_KKILL_HPP

// Reasons why the kernel might have killed a process
// A negative exit value can be one of these

namespace std {
	struct kkill {
		enum {
			SEGFAULT, // Access to non-mapped memory
			BAD_STRING, // Process sent the kernel an invalid string
			LOADER_SYSCALL, // Tried to use a loader-only syscall
			UNKNOWN_SYSCALL
		};
	};
};

#endif
