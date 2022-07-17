#ifndef _STDLIB_USERSPACE_BLOCK_HPP
#define _STDLIB_USERSPACE_BLOCK_HPP

#include <types>

namespace std {
	struct block {
		enum {
			LIST_DEVICES,
			SELECT,
			READ,
			WRITE,
		};

		struct DeviceTypes {
			enum {
				NONE,
				AHCIATAPI,
				RAMBLOCK,
			};
		};
	};
};

#endif
