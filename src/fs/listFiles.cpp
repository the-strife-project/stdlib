#include "fs.hpp"
#include <rpc>

// I'm not proud of this at all

size_t std::listFiles(const std::string& path, FileList& ret) {
	if(!_fs_isSelected || path != _fs_selected) {
		size_t sel = _fs_select(path + "/");
		if(sel != std::VFS::SELECT_OK)
			return sel;
	}

	std::list<uint8_t*> pages;
	while(true) {
		if(!std::rpc(_fs_vfs, std::VFS::LIST, pages.size()))
			break;

		uint8_t* page = new uint8_t[PAGE_SIZE];
		memcpy(page, _fs_shared, PAGE_SIZE);
		pages.push_back(page);
	}

	uint8_t* data = new uint8_t[pages.size() * PAGE_SIZE];
	uint8_t* ptr = data;
	for(auto const& x : pages) {
		memcpy(ptr, x, PAGE_SIZE);
		delete [] x;
	}

	// We finally got data
	ptr = data;
	while(ptr < data + pages.size() * PAGE_SIZE) {
		uint64_t inode = *(uint64_t*)ptr;
		ptr += sizeof(uint64_t);

		if(!inode)
			break;

		uint64_t namesz = *(uint64_t*)ptr;
		ptr += sizeof(uint64_t);
		char* name = new char[namesz + 1];
		memcpy(name, ptr, namesz);
		name[namesz] = 0;
		ptr += namesz;

		ret.add(name);
	}

	return std::FS_OK;
}
