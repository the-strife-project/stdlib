#include "fs.hpp"
#include <rpc>
#include <shared_memory>

std::VFS::Info std::getFileInfo(const std::string& path) {
	if(!std::_fs_isSelected || path != std::_fs_selected) {
		size_t sel = std::_fs_select(path);
		if(sel != std::VFS::OK)
			return {sel};
	}

	std::SMID smid = std::smMake();
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, std::_fs_vfs);

	auto* ptr = (std::VFS::Info*)buffer;
	*ptr = std::VFS::Info();

	if(!std::rpc(std::_fs_vfs, std::VFS::INFO, smid)) {
		std::munmap(buffer);
		std::smDrop(smid);
		return {std::VFS::ERROR_READING};
	}

	auto ret = *ptr;
	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}
