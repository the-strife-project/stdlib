#ifndef _STDLIB_REGISTRY_HPP
#define _STDLIB_REGISTRY_HPP

#include <string>
#include <types>

namespace std {
	namespace registry {
		extern std::PID _pid;

		size_t exists(std::string& path);
		bool has(std::PID pid, const std::string& name);
	};
};

#endif
