#ifndef _STDLIB_STRIFEFS_HPP
#define _STDLIB_STRIFEFS_HPP

namespace std {
	struct StrifeFS {
		enum {
			SETUP,
			GET_INODE,
			READ,
			WRITE,
			MAKE_FILE,
			MAKE_DIR,
		};
	};
};

#endif
