#include "syscalls.hpp"

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

bool std::publish(const std::string& name) {
	return _syscallOne(Syscalls::PUBLISH, str2u64(name));
}

PID std::resolve(const std::string& name) {
	return _syscallOne(Syscalls::RESOLVE, str2u64(name));
}
