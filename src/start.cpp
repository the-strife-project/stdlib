#include <common.hpp>

extern "C" void _start() {
	asm volatile("syscall");
}
