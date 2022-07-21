#ifndef _STDLIB_USERSPACE_REGISTRY_HPP
#define _STDLIB_USERSPACE_REGISTRY_HPP

namespace std {
	struct uregistry {
		enum {
			EXISTS,
			CREATE,
		};

		enum {
			OK,
			CONNECTION_ERROR,
			NOT_ALLOWED,
			NOT_FOUND,
		};
	};
};

#endif
