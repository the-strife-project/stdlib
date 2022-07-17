#ifndef _STDLIB_CSTDIO_HPP
#define _STDLIB_CSTDIO_HPP

#include <stdarg.h>
#include <rpc>
#include <userspace/term.hpp>

namespace std {
	extern size_t _buffered;
	extern char* _bufferedOut;
	extern std::PID _term;

	inline size_t flushTerm() {
		size_t ret = rpc(std::_term, std::term::FLUSH, std::_buffered);
		std::_buffered = 0;
		return ret;
	}

	inline void clearTerm() { rpc(std::_term, std::term::CLEAR); }

	size_t printf(const char* fmt, ...);

	char* _hexToStr(uint64_t x, char* buffer, size_t fixedSize=0);
	char* _u64ToStr(uint64_t x, char* buffer, size_t fixedSize=0);
}

#endif
