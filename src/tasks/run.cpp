#include "tasks.hpp"
#include <fs>
#include <syscalls>

std::PID std::run(const string& path, const Args& args, const Env& env) {
	Buffer buffer;
	if(readWholeFile(path, buffer) != FS_OK)
		return NULL_PID;

	void* send = mmap(NPAGES(buffer.size()));
	if(!send)
		return NULL_PID;
	memcpy(send, buffer.get(), buffer.size());

	// Prepare runtime
	// How much space do I need?
	size_t rtsz = 2; // The two null strings
	for(auto& x : args)
		rtsz += x.size() + 1;
	for(auto& x : env)
		rtsz += x.f.size() + x.s.size() + 2;

	uint8_t* runtime = (uint8_t*)mmap(NPAGES(rtsz));
	uint8_t* cur = runtime;
	for(auto& x : args) {
		std::string aux = x;
		memcpy(cur, aux.c_str(), aux.size());
		cur += aux.size() + 1;
	}
	++cur; // Empty string as separator
	for(auto& x : env) {
		std::string aux = x.f;
		memcpy(cur, aux.c_str(), aux.size());
		cur += aux.size();
		*(cur++) = '=';
		aux = x.s;
		memcpy(cur, aux.c_str(), aux.size());
		cur += aux.size() + 1;
	}

	return _exec(send, buffer.size(), runtime, rtsz);
}
