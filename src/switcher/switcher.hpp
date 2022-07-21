#ifndef _STDLIB_SWITCHER_HPP
#define _STDLIB_SWITCHER_HPP

#include <types>

namespace std {
	struct switcher {
		enum {
			SWITCH,
		};

		enum {
			OK,
			CONNECTION_ERROR,
			NOT_FOUND,
			NOT_ALLOWED,
		};
	};

	size_t switchUser(size_t uid);
};

#endif
