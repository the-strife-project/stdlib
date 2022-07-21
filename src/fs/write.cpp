#include "fs.hpp"
#include <rpc>
#include <shared_memory>

size_t std::writeFile(const std::string& path, char* data, size_t start, size_t sz) {
	if(!_fs_isSelected || path != _fs_selected) {
		size_t sel = _fs_select(path);
		if(sel != std::VFS::OK)
			return sel;
	}

	size_t npages = NPAGES(sz);
	std::SMID smid = std::smMake(npages);
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, _fs_vfs);
	memcpy(buffer, data, sz);

	auto ret = std::rpc(_fs_vfs, std::VFS::WRITE, smid, start, sz);

	std::munmap(buffer, npages);
	std::smDrop(smid);
	return ret;
}
