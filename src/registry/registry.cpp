#include "registry.hpp"
#include <rpc>
#include <shared_memory>
#include <userspace/registry.hpp>
#include <cstdio>

std::PID std::registry::_pid = NULL_PID;

static size_t _resolve() {
	if(std::registry::_pid)
		return std::registry::_pid;
	return std::registry::_pid = std::resolve("registry");
}

size_t std::registry::exists(std::string& path) {
	if(path.size() >= PAGE_SIZE)
		return std::uregistry::CONNECTION_ERROR;

	std::PID pid = _resolve();
	if(!pid)
		return std::uregistry::CONNECTION_ERROR;

	std::SMID smid = std::smMake();
	char* buffer = (char*)std::smMap(smid);
	std::smAllow(smid, pid);

	memcpy(buffer, path.c_str(), path.size());
	auto ret = std::rpc(pid, std::uregistry::EXISTS, smid);

	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}

std::unordered_set<std::string> std::registry::list(std::string& path) {
	if(path.size() >= PAGE_SIZE)
		return {};

	std::PID pid = _resolve();
	if(!pid)
		return {};

	std::SMID smid = std::smMake();
	char* buffer = (char*)std::smMap(smid);
	std::smAllow(smid, pid);
	memcpy(buffer, path.c_str(), path.size());

	size_t sz = std::rpc(pid, std::uregistry::LIST_SIZE, smid);
	std::munmap(buffer);
	std::smDrop(smid);

	if(!sz)
		return {};

	size_t npages = NPAGES(sz);
	smid = std::smMake(npages);
	buffer = (char*)std::smMap(smid);
	std::smAllow(smid, pid);
	memcpy(buffer, path.c_str(), path.size());

	bool err = std::rpc(pid, std::uregistry::LIST, smid);
	if(!err) {
		std::munmap(buffer, npages);
		std::smDrop(smid);
		return {};
	}

	std::unordered_set<std::string> ret;
	char* ptr = buffer;
	char* limit = ptr + sz;
	while(ptr < limit) {
		std::string name(ptr);
		ptr += name.size() + 1;
		ret.add(name);
	}

	std::munmap(buffer, npages);
	std::smDrop(smid);
	return ret;
}

size_t std::registry::create(std::string& path) {
	if(path.size() >= PAGE_SIZE)
		return std::uregistry::CONNECTION_ERROR;

	std::PID pid = _resolve();
	if(!pid)
		return std::uregistry::CONNECTION_ERROR;

	std::SMID smid = std::smMake();
	char* buffer = (char*)std::smMap(smid);
	std::smAllow(smid, pid);

	memcpy(buffer, path.c_str(), path.size());
	auto ret = std::rpc(pid, std::uregistry::CREATE, smid);

	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}



bool std::registry::has(std::PID pid, const std::string& name) {
	auto info = std::info(pid);
	size_t uid = info.uid;
	if(!uid)
		return false;
	if(uid == 1)
		return true;

	std::string path = "/u/";
	path += uToStr(uid);
	path += "/";
	path += name;

	auto ret = exists(path);
	// In case of connection error (no registry running), everything is allowed!
	return ret != std::uregistry::NOT_FOUND;
}
