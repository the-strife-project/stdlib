#include "VFS.hpp"

const char* std::VFS::errStr(size_t n) {
	// Shenanigans because of lack of global constructors (TODO)
	switch(n) {
	case SELECT_CONNECT_ERROR:
		return "Connection error";
	case SELECT_NOT_ALLOWED:
		return "Permission denied";
	case SELECT_NOT_FOUND:
		return "No such file or directory";
	case ERROR_READING:
		return "Read fault";
	}

	return "";
}
