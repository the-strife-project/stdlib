#ifndef _STDLIB_FS_HPP
#define _STDLIB_FS_HPP

#include <string>
#include <unordered_set>
#include <userspace/VFS.hpp>

namespace std {
	typedef unordered_set<std::string> FileList;
	const size_t FS_OK = VFS::SELECT_OK;

	// Private-ish
	extern std::PID _fs_vfs;
	extern char* _fs_shared;
	extern bool _fs_isSelected;
	extern std::string _fs_selected;
	size_t _fs_select(const std::string&);

	// Regular
	size_t listFiles(const std::string&, FileList&);
};

#endif
