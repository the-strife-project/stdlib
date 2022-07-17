#ifndef _STDLIB_USERSPACE_RAMBLOCK_HPP
#define _STDLIB_USERSPACE_RAMBLOCK_HPP

#include <types>

namespace std {
	struct ramblock {
		enum {
			READ,
			WRITE,
		};
	};
};

#endif
