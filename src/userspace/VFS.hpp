#ifndef _STDLIB_VFS_HPP
#define _STDLIB_VFS_HPP

namespace std {
	struct VFS {
		enum {
			CONNECT,
			SELECT,
			LIST,
			READ,
			INFO,
		};

		enum {
			SELECT_OK,
			SELECT_CONNECT_ERROR,
			SELECT_NOT_ALLOWED,
			SELECT_NOT_FOUND,
			ERROR_READING,
		};

		struct Info {
			size_t size;
			bool isDirectory;
		};
	};
};

#endif