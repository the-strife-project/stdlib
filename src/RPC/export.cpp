#include "export.hpp"
#include <pair>
#include <unordered_map>
#include <syscalls>

// Pair of a function pointer and the number of arguments it receives
typedef std::pair<uint64_t, size_t> funcargs_t;
static std::vector<funcargs_t> procs = std::vector<funcargs_t>();

size_t std::exportProcedure(void* func, size_t args) {
	procs.push_back(funcargs_t((uint64_t)func, args));
	return procs.size()-1;
}

// This is the famous rcx=r10 I was talking about in kernel's rpcSwitcher.asm
asm(".global rpcEntry\n"
	".type rpcEntry, @function\n"
	"rpcEntry:\n"
	"pop %rcx\n"
	"call _ZSt10rpcHandlertmmmmm\n"
	// Return
	"mov %rsi, %rbp\n" // Return value
	"pop %rax" /* Fire page fault */);

uint64_t std::rpcHandler(PID client, RPID id, uint64_t arg0, uint64_t arg1,
						 uint64_t arg2, uint64_t arg3) {
	if(id >= procs.size())
		return ~0;

	const funcargs_t& funcargs = procs[id];
	union {
		uint64_t u64;
		size_t (*zero)(size_t);
		size_t (*one)(size_t, size_t);
		size_t (*two)(size_t, size_t, size_t);
		size_t (*three)(size_t, size_t, size_t, size_t);
		size_t (*four)(size_t, size_t, size_t, size_t, size_t);
	} fptr;

	fptr.u64 = funcargs.f;

	switch(funcargs.s) { // Number of arguments
	case 0: return fptr.zero(client);
	case 1: return fptr.one(client, arg0);
	case 2: return fptr.two(client, arg0, arg1);
	case 3: return fptr.three(client, arg0, arg1, arg2);
	case 4: return fptr.four(client, arg0, arg1, arg2, arg3);
	}

	return ~0;
}
