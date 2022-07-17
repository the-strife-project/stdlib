#ifndef _STDLIB_USERSPACE_AHCI_HPP
#define _STDLIB_USERSPACE_AHCI_HPP

#include <types>

namespace std {
	struct AHCI {
		enum {
			GET_ATAPIS,
			READ_ATAPI,
		};
	};
};

#endif
