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
			MKDIR,
			MKFILE,
		};

		enum {
			OK,
			CONNECT_ERROR,
			NOT_ALLOWED,
			NOT_FOUND,
			ERROR_READING,
			ALREADY_EXISTS,
			READ_ONLY_FS,
			NOT_A_DIRECTORY,
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
