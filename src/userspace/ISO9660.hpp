#ifndef _STDLIB_USERSPACE_ISO9660_HPP
#define _STDLIB_USERSPACE_ISO9660_HPP

#include <types>

namespace std {
	struct ISO9660 {
		enum {
			SETUP,
			GET_ROOT,
			LIST_SIZE,
			LIST,
			READ,
		};
	};
};

#endif
