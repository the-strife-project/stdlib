#ifndef _STDLIB_STRIFEFS_HPP
#define _STDLIB_STRIFEFS_HPP

namespace std {
	struct StrifeFS {
		enum {
			SETUP,
			CONNECT,
			GET_INODE,
			READ,
			WRITE,
		};
	};
};

#endif
