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
			WRITE,
			INFO,
			MKDIR,
			MKFILE,
			ADD_ACL,
			ACL_SIZE,
			GET_ACL,
			EACL_SIZE,
			GET_EACL,
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
			size_t error = ERROR_READING;
			size_t size = 0;
			bool isDirectory = false;

			Info() = default;
			inline Info(size_t err) { error = err; }
		};
	};
};

#endif
