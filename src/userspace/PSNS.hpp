#ifndef _STDLIB_USERSPACE_PSNS_HPP
#define _STDLIB_USERSPACE_PSNS_HPP

#include <types>

namespace std {
	struct PSNS {
		enum {
			PUBLISH,
			RESOLVE,
		};
	};
};

#endif
