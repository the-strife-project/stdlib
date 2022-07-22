#include "fs.hpp"
#include <rpc>
#include <shared_memory>

size_t std::readFile(const std::string& path, uint8_t* data, size_t start, size_t sz) {
	if(!_fs_isSelected || path != _fs_selected) {
		size_t sel = _fs_select(path);
		if(sel != std::VFS::OK)
			return sel;
	}

	size_t npages = NPAGES(sz);
	std::SMID smid = std::smMake(npages);
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, _fs_vfs);

	if(!rpc(_fs_vfs, std::VFS::READ, smid, start, sz)) {
		std::munmap(buffer);
		std::smDrop(smid);
		return std::VFS::ERROR_READING;
	}

	memcpy(data, buffer, sz);

	std::munmap(buffer, npages);
	std::smDrop(smid);
	return std::FS_OK;
}

size_t std::readWholeFile(const std::string& path, std::Buffer& ref, bool nullTerminated) {
	auto info = std::getFileInfo(path);
	if(info.error != FS_OK)
		return info.error;
	size_t size = info.size;

	uint8_t* raw;
	size_t allocSize = size;
	if(nullTerminated) {
		++allocSize;
		raw = new uint8_t[allocSize];
		raw[size] = 0;
	} else {
		raw = new uint8_t[allocSize];
	}

	Buffer buff = {raw, allocSize};
	readFile(path, buff.get(), 0, size);
	ref = buff;

	return std::FS_OK;
}
