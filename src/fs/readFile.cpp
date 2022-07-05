#include "fs.hpp"
#include <rpc>

size_t std::readFile(const std::string& path, uint8_t* data, size_t start, size_t sz) {
	if(!_fs_isSelected || path != _fs_selected) {
		size_t sel = _fs_select(path);
		if(sel != std::VFS::SELECT_OK)
			return sel;
	}

	uint8_t* ptr = data;
	size_t npages = (sz + PAGE_SIZE - 1) / PAGE_SIZE;
	size_t page = start / PAGE_SIZE;
	size_t off = start % PAGE_SIZE;

	while(npages--) {
		if(!rpc(_fs_vfs, std::VFS::READ, page))
			return std::VFS::ERROR_READING;

		size_t copy = std::min(PAGE_SIZE - off, sz);
		memcpy(ptr, _fs_shared+off, copy);

		off = 0;
		++page;
		ptr += copy;
		sz -= copy;
	}

	return std::FS_OK;
}

size_t std::readWholeFile(const std::string& path, std::Buffer& ref) {
	if(!_fs_isSelected || path != _fs_selected) {
		size_t sel = _fs_select(path);
		if(sel != std::VFS::SELECT_OK)
			return sel;
	}

	if(!std::rpc(_fs_vfs, std::VFS::INFO))
		return std::VFS::ERROR_READING;

	std::VFS::Info info;
	memcpy(&info, _fs_shared, sizeof(info));
	size_t size = info.size;

	Buffer buffer = {new uint8_t[size], size};
	readFile(path, buffer.get(), 0, size);
	ref = buffer;
	return std::FS_OK;
}
