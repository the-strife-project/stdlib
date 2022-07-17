#include "fs.hpp"
#include <rpc>
#include <shared_memory>

size_t std::listFiles(const std::string& path, FileList& ret) {
	if(!_fs_isSelected || path != _fs_selected) {
		size_t sel = _fs_select(path + "/");
		if(sel != std::VFS::SELECT_OK)
			return sel;
	}

	size_t npages = std::rpc(_fs_vfs, std::VFS::LIST_SIZE);

	std::SMID smid = std::smMake(npages);
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, _fs_vfs);

	if(!std::rpc(_fs_vfs, std::VFS::LIST, smid)) {
		std::munmap(buffer, npages);
		std::smDrop(smid);
		return 0;
	}

	uint8_t* ptr = buffer;
	while(ptr < buffer + npages * PAGE_SIZE) {
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
