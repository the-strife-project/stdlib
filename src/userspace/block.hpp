#ifndef _STDLIB_USERSPACE_BLOCK_HPP
#define _STDLIB_USERSPACE_BLOCK_HPP

#include <types>

namespace std {
	struct block {
		enum {
			CONNECT,
			LIST_DEVICES,
			READ,
		};
	};
};

#endif
