#include "fs.hpp"
#include <rpc>
#include <shared_memory>

size_t std::addACL(const std::string& path, size_t uid, const std::ACLEntry& acl) {
	if(!std::_fs_isSelected || path != std::_fs_selected) {
		size_t sel = std::_fs_select(path);
		if(sel != std::VFS::OK)
			return {sel};
	}

	bool ret = std::rpc(std::_fs_vfs, std::VFS::ADD_ACL, uid, acl.raw);
	if(ret)
		return std::FS_OK;
	else
		return std::VFS::ERROR_READING;
}

size_t std::getACL(const std::string& path, std::ACL& acl) {
	if(!std::_fs_isSelected || path != std::_fs_selected) {
		size_t sel = std::_fs_select(path);
		if(sel != std::VFS::OK)
			return {sel};
	}

	acl = ACL();

	size_t size = std::rpc(std::_fs_vfs, std::VFS::ACL_SIZE);
	if(!size)
		return std::FS_OK;

	size_t npages = NPAGES(size);

	std::SMID smid = std::smMake(npages);
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, std::_fs_vfs);

	bool result = std::rpc(std::_fs_vfs, std::VFS::GET_ACL, smid);
	if(!result) {
		std::munmap(buffer, npages);
		std::smDrop(smid);
		return std::VFS::CONNECT_ERROR;
	}

	size_t n = size / (2 * sizeof(uint64_t));
	uint64_t* ptr = (uint64_t*)buffer;
	while(n--) {
		size_t uid = *(ptr++);
		ACLEntry entry;
		entry.raw = *(ptr++);
		acl[uid] = entry;
	}

	std::munmap(buffer, npages);
	std::smDrop(smid);
	return std::FS_OK;
}

size_t std::getEACL(const std::string& path, std::ACL& acl) {
	if(!std::_fs_isSelected || path != std::_fs_selected) {
		size_t sel = std::_fs_select(path);
		if(sel != std::VFS::OK)
			return {sel};
	}

	acl = ACL();

	size_t size = std::rpc(std::_fs_vfs, std::VFS::EACL_SIZE);
	if(!size)
		return std::FS_OK;

	size_t npages = NPAGES(size);

	std::SMID smid = std::smMake(npages);
	uint8_t* buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, std::_fs_vfs);

	bool result = std::rpc(std::_fs_vfs, std::VFS::GET_EACL, smid);
	if(!result) {
		std::munmap(buffer, npages);
		std::smDrop(smid);
		return std::VFS::CONNECT_ERROR;
	}

	size_t n = size / (2 * sizeof(uint64_t));
	uint64_t* ptr = (uint64_t*)buffer;
	while(n--) {
		size_t uid = *(ptr++);
		ACLEntry entry;
		entry.raw = *(ptr++);
		acl[uid] = entry;
	}

	std::munmap(buffer, npages);
	std::smDrop(smid);
	return std::FS_OK;
}
