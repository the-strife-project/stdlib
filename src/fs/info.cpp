#include "fs.hpp"
#include <rpc>
#include <shared_memory>

template<bool v> static bool isSomething(const std::string& path) {
	if(!std::_fs_isSelected || path != std::_fs_selected) {
		size_t sel = std::_fs_select(path);
		if(sel != std::VFS::OK)
			return false;
	}

	std::SMID smid = std::smMake();
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, std::_fs_vfs);

	if(!std::rpc(std::_fs_vfs, std::VFS::INFO, smid)) {
		std::munmap(buffer);
		std::smDrop(smid);
		return false;
	}

	auto* info = (std::VFS::Info*)buffer;
	bool ret = v ^ info->isDirectory;

	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}

bool std::isFile(const std::string& path) {
	return isSomething<true>(path);
}

bool std::isDir(const std::string& path) {
	return isSomething<false>(path);
}
