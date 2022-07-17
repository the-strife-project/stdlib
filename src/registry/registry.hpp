#ifndef _STDLIB_REGISTRY_HPP
#define _STDLIB_REGISTRY_HPP

#include <string>
#include <types>

namespace std {
	namespace registry {
		extern std::PID _pid;
		extern char* _buffer;

		size_t exists(const std::string& path);
	};
};

#endif
