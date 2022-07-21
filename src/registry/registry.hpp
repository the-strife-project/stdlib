#ifndef _STDLIB_REGISTRY_HPP
#define _STDLIB_REGISTRY_HPP

#include <string>
#include <types>
#include <unordered_set>
#include <userspace/registry.hpp>

namespace std {
	namespace registry {
		const size_t OK = std::uregistry::OK;
		extern std::PID _pid;

		size_t exists(std::string& path);
		std::unordered_set<std::string> list(std::string& path);
		size_t create(std::string& path);

		bool has(std::PID pid, const std::string& name);
	};
};

#endif
