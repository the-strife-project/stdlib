#ifndef _STDLIB_FS_HPP
#define _STDLIB_FS_HPP

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <userspace/VFS.hpp>
#include <memory>

namespace std {
	typedef unordered_set<std::string> FileList;
	const size_t FS_OK = VFS::OK;

	// Private-ish
	extern std::PID _fs_vfs;
	extern bool _fs_isSelected;
	extern std::string _fs_selected;
	size_t _fs_select(const std::string&);

	// Regular
	inline bool exists(const std::string& path) {
		return _fs_select(path) == VFS::OK;
	}

	std::string simplifyPath(const std::string&);

	std::VFS::Info getFileInfo(const std::string&);
	inline bool isFile(const std::string& path) {
		auto info = getFileInfo(path);
		if(info.error != FS_OK)
			return false;
		return !info.isDirectory;
	}
	inline bool isDir(const std::string& path) {
		auto info = getFileInfo(path);
		if(info.error != FS_OK)
			return false;
		return info.isDirectory;
	}

	size_t listFiles(const std::string&, FileList&);
	size_t readFile(const std::string&, uint8_t*, size_t start, size_t sz);
	size_t readWholeFile(const std::string&, Buffer&, bool nullTerminated=false);
	size_t writeFile(const std::string&, char*, size_t start, size_t sz);

	size_t mkdir(const std::string&);
	size_t mkfile(const std::string&);

	struct ACLEntry {
		union {
			struct {
				uint64_t allow : 1;
				uint64_t isUser : 1;
				uint64_t read : 1;
				uint64_t write : 1;
				uint64_t reserved : 60;
			};
			uint64_t raw;
		};
	} __attribute__((packed));
	typedef std::unordered_map<size_t, ACLEntry> ACL;

	size_t addACL(const std::string&, size_t uid, const ACLEntry&);
	size_t getACL(const std::string&, ACL&);
	size_t getEACL(const std::string&, ACL&);
};

#endif
