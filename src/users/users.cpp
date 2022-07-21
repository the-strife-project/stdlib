#include "users.hpp"
#include <shared_memory>
#include <rpc>

std::PID std::_users_pid = NULL_PID;

static size_t _resolve() {
	if(std::_users_pid)
		return std::_users_pid;
	return std::_users_pid = std::resolve("users");
}

std::string std::uidToName(size_t uid) {
	std::PID pid = _resolve();
	if(!pid)
		return "";

	std::SMID smid = std::smMake();
	char* buffer = (char*)std::smMap(smid);
	std::smAllow(smid, pid);

	if(!std::rpc(pid, std::users::GET_NAME, smid, uid)) {
		std::munmap(buffer);
		std::smDrop(smid);
		return "";
	}

	std::string ret(buffer);
	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}

size_t std::nameToUID(std::string& name) {
	std::PID pid = _resolve();
	if(!pid)
		return 0;

	if(name.size() >= PAGE_SIZE)
		return 0;

	std::SMID smid = std::smMake();
	char* buffer = (char*)std::smMap(smid);
	std::smAllow(smid, pid);
	memcpy(buffer, name.c_str(), name.size());

	size_t ret = std::rpc(pid, std::users::GET_UID, smid);

	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}

size_t std::howManyUsers() {
	std::PID pid = _resolve();
	if(!pid)
		return 0;

	return std::rpc(pid, std::users::GET_COUNT);
}

size_t std::newUser(std::string& name) {
	std::PID pid = _resolve();
	if(!pid)
		return 0;

	if(name.size() >= PAGE_SIZE)
		return 0;

	std::SMID smid = std::smMake();
	char* buffer = (char*)std::smMap(smid);
	std::smAllow(smid, pid);
	memcpy(buffer, name.c_str(), name.size());

	auto ret = std::rpc(pid, std::users::NEW_USER, smid);

	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}
