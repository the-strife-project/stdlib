#include "switcher.hpp"
#include <rpc>

size_t std::switchUser(size_t uid) {
	std::PID pid = std::resolve("switcher");
	if(!pid)
		return std::switcher::CONNECTION_ERROR;

	return std::rpc(pid, std::switcher::SWITCH, uid);
}
