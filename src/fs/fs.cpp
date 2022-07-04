#include "fs.hpp"
#include <rpc>
#include <shared_memory>
#include <userspace/VFS.hpp>

std::PID std::_fs_vfs = 0;
char* std::_fs_shared = nullptr;

static bool connect() {
	if(std::_fs_shared)
		return true;

	std::_fs_vfs = std::resolve("VFS");
	if(!std::_fs_vfs)
		return false;

	std::SMID smid = std::smMake();
	std::_fs_shared = (char*)std::smMap(smid);
	std::smAllow(smid, std::_fs_vfs);
	return std::rpc(std::_fs_vfs, std::VFS::CONNECT, smid);
}

bool std::_fs_isSelected = false;
std::string std::_fs_selected;

size_t std::_fs_select(const std::string& path) {
	if(!connect())
		return false;

	if(path.size() >= PAGE_SIZE)
		return false;

	_fs_selected = path;
	memcpy(_fs_shared, _fs_selected.c_str(), _fs_selected.size());
	size_t ret = std::rpc(_fs_vfs, std::VFS::SELECT, _fs_selected.size());
	_fs_isSelected = (ret == std::VFS::SELECT_OK);

	return ret;
}

// read

// write

// mkfile

// mkdir
