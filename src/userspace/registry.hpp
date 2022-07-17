#ifndef _STDLIB_USERSPACE_REGISTRY_HPP
#define _STDLIB_USERSPACE_REGISTRY_HPP

namespace std {
	struct uregistry {
		enum {
			CONNECT,
			EXISTS,
			CREATE,
		};

		enum {
			OK,
			CONNECTION_ERROR,
			BADLY_FORMED, // borrame
			NOT_FOUND,
		};
	};
};

#endif
