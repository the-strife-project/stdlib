#include "fs.hpp"
#include <rpc>
#include <shared_memory>

size_t std::mkdir(const std::string& path) {
	auto parts = std::simplifyPath(path).split('/');
	if(!parts.size())
		return std::VFS::CONNECT_ERROR;

	std::string name = parts[parts.size()-1];
	parts.pop_back();

	std::string parent = "";
	for(auto const& x : parts) {
		parent += x;
		parent += "/";
	}

	if(!_fs_isSelected || parent != _fs_selected) {
		size_t sel = _fs_select(parent);
		if(sel != std::VFS::OK)
			return sel;
	}

	std::SMID smid = std::smMake();
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, _fs_vfs);

	memcpy(buffer, name.c_str(), name.size());
	size_t ret = rpc(_fs_vfs, std::VFS::MKDIR, smid);

	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}
