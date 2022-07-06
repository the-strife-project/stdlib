#include "tasks.hpp"
#include <fs>
#include <syscalls>

std::PID std::run(const string& path) {
	Buffer buffer;
	if(readWholeFile(path, buffer) != FS_OK)
		return NULL_PID;

	size_t npages = (buffer.size() + PAGE_SIZE - 1) / PAGE_SIZE;
	void* send = mmap(npages);
	if(!send)
		return NULL_PID;

	memcpy(send, buffer.get(), buffer.size());
	return _exec(send, buffer.size());
}
