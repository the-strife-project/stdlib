#include "registry.hpp"
#include <rpc>
#include <shared_memory>
#include <userspace/registry.hpp>

std::PID std::registry::_pid = NULL_PID;
char* std::registry::_buffer = nullptr;

static size_t connect() {
	if(std::registry::_pid)
		return std::uregistry::OK;

	std::registry::_pid = std::resolve("registry");
	if(!std::registry::_pid)
		return std::uregistry::CONNECTION_ERROR;

	std::SMID smid = std::smMake();
	std::registry::_buffer = (char*)std::smMap(smid);
	std::smAllow(smid, std::registry::_pid);
	return std::rpc(std::registry::_pid, std::uregistry::CONNECT, smid);
}

size_t std::registry::exists(const std::string& path) {
	size_t ret = connect();
	if(ret != std::uregistry::OK)
		return ret;

	if(path.size() >= PAGE_SIZE)
		return std::uregistry::CONNECTION_ERROR;

	std::string copy(path);
	memcpy(_buffer, copy.c_str(), copy.size());

	return std::rpc(_pid, std::uregistry::EXISTS);
}
