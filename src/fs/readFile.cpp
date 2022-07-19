#include "fs.hpp"
#include <rpc>
#include <shared_memory>

size_t std::readFile(const std::string& path, uint8_t* data, size_t start, size_t sz) {
	if(!_fs_isSelected || path != _fs_selected) {
		size_t sel = _fs_select(path);
		if(sel != std::VFS::SELECT_OK)
			return sel;
	}

	uint8_t* ptr = data;
	size_t npages = NPAGES(sz);
	size_t page = PAGE(start);
	size_t off = PAGEOFF(start);

	std::SMID smid = std::smMake();
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, _fs_vfs);

	while(npages--) {
		if(!rpc(_fs_vfs, std::VFS::READ, smid, page)) {
			std::munmap(buffer);
			std::smDrop(smid);
			return std::VFS::ERROR_READING;
		}

		size_t copy = std::min(PAGE_SIZE - off, sz);
		memcpy(ptr, buffer+off, copy);

		off = 0;
		++page;
		ptr += copy;
		sz -= copy;
	}

	std::munmap(buffer);
	std::smDrop(smid);
	return std::FS_OK;
}

size_t std::readWholeFile(const std::string& path, std::Buffer& ref) {
	if(!_fs_isSelected || path != _fs_selected) {
		size_t sel = _fs_select(path);
		if(sel != std::VFS::SELECT_OK)
			return sel;
	}

	std::SMID smid = std::smMake();
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, _fs_vfs);

	if(!std::rpc(_fs_vfs, std::VFS::INFO, smid)) {
		std::munmap(buffer);
		std::smDrop(smid);
		return std::VFS::ERROR_READING;
	}

	auto* info = (std::VFS::Info*)buffer;
	size_t size = info->size;

	std::munmap(buffer);
	std::smDrop(smid);

	Buffer buff = {new uint8_t[size], size};
	readFile(path, buff.get(), 0, size);
	ref = buff;

	return std::FS_OK;
}
