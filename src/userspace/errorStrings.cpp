#include "VFS.hpp"

const char* std::VFS::errStr(size_t n) {
	// Shenanigans because of lack of global constructors (TODO)
	switch(n) {
	case CONNECT_ERROR:
		return "Connection error";
	case NOT_ALLOWED:
		return "Permission denied";
	case NOT_FOUND:
		return "No such file or directory";
	case ERROR_READING:
		return "Read fault";
	case ALREADY_EXISTS:
		return "File already exists";
	case READ_ONLY_FS:
		return "Filesystem is read-only";
	case NOT_A_DIRECTORY:
		return "Not a directory";
	}

	return "";
}
