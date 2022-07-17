#include "fs.hpp"
#include <rpc>
#include <shared_memory>
#include <userspace/VFS.hpp>

std::PID std::_fs_vfs = 0;

static bool _resolve() {
	if(std::_fs_vfs)
		return true;

	std::_fs_vfs = std::resolve("VFS");
	if(!std::_fs_vfs)
		return false;
	return true;
}

bool std::_fs_isSelected = false;
std::string std::_fs_selected;

size_t std::_fs_select(const std::string& path) {
	if(!_resolve())
		return false;

	if(path.size() >= PAGE_SIZE)
		return false;

	std::SMID smid = std::smMake();
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, _fs_vfs);

	_fs_selected = path;
	memcpy(buffer, _fs_selected.c_str(), _fs_selected.size());
	size_t ret = std::rpc(_fs_vfs, std::VFS::SELECT, smid, _fs_selected.size());
	_fs_isSelected = (ret == std::VFS::SELECT_OK);

	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}

// write

// mkfile

// mkdir
