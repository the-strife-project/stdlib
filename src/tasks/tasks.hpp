#ifndef _STDLIB_TASKS_HPP
#define _STDLIB_TASKS_HPP

#include <string>
#include <unordered_map>

namespace std {
	typedef std::vector<std::string> Args;
	typedef std::unordered_map<std::string, std::string> Env;

	PID run(const string& path, const Args&, const Env&);

	struct RuntimeV { Args args; Env env; };
	class Runtime {
	private:
		uint8_t* raw;

	public:
		RuntimeV parse();
	};
};

#endif
