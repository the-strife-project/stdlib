#ifndef _STDLIB_VFS_HPP
#define _STDLIB_VFS_HPP

#include <types>

namespace std {
	struct VFS {
		enum {
			SELECT,
			LIST_SIZE,
			LIST,
			READ,
			INFO,
			MAKE_FILE,
			MAKE_DIR,
		};

		enum {
			SELECT_OK,
			SELECT_CONNECT_ERROR,
			SELECT_NOT_ALLOWED,
			SELECT_NOT_FOUND,
			ERROR_READING,
			N_ERRORS,
		};

		static const char* errStr(size_t);

		struct Info {
			size_t size;
			bool isDirectory;
		};
	};
};

#endif
