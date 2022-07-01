#include "syscalls.hpp"
#include <rpc>
#include <userspace/PSNS.hpp>

static std::PID psns = 0;

static uint64_t str2u64(const std::string& name) {
	if(name.size() > 8)
		return 0;

	uint64_t ret = 0;
	for(char c : name) {
		ret <<= 8;
		ret |= c;
	}

	return ret;
}

static void fillPSNSpid() {
	if(!psns)
		psns = std::_syscallZero(std::Syscalls::FIND_PSNS);
}

bool std::publish(const std::string& name) {
	fillPSNSpid();
	return rpc(psns, PSNS::PUBLISH, str2u64(name));
}

std::PID std::resolve(const std::string& name) {
	fillPSNSpid();
	return rpc(psns, PSNS::RESOLVE, str2u64(name));
}
