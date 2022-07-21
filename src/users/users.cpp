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
		return 0;

	std::SMID smid = std::smMake();
	char* buffer = (char*)std::smMap(smid);
	std::smAllow(smid, pid);

	if(!std::rpc(pid, std::users::GET_NAME, smid, uid)) {
		std::munmap(buffer);
		std::smDrop(smid);
		return 0;
	}

	std::string ret(buffer);
	std::munmap(buffer);
	std::smDrop(smid);
	return ret;
}
