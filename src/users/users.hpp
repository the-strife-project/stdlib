#ifndef _STDLIB_USERS_HPP
#define _STDLIB_USERS_HPP

#include <userspace/users.hpp>
#include <string>

namespace std {
	extern std::PID _users_pid;

	std::string uidToName(size_t);
	size_t nameToUID(std::string&);
};

#endif
