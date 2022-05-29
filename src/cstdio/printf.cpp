#include <cstdio>
#include <shared_memory>
#include <rpc>

size_t std::_buffered = 0;
char* std::_bufferedOut = nullptr;
std::PID std::_term = 0;

static void _writec(char c) {
	std::_bufferedOut[std::_buffered++] = c;

	if(c == '\n' || std::_buffered == 4095)
		std::flushTerm();
}

static inline size_t _writes(const char* str) {
	size_t ctr = 0;
	for(; *str; ++str, ++ctr)
		_writec(*str);
	return ctr;
}

size_t std::printf(const char* fmt, ...) {
	// TODO: Maybe a lock here?
	if(!std::_bufferedOut) {
		// Initialize shared memory with term
		std::SMID smid = std::smMake();
		std::_bufferedOut = (char*)std::smMap(smid);

		std::_term = std::resolve("term");
		std::smAllow(smid, std::_term);
		std::rpc(std::_term, _STDLIB_TERM_ID_CONNECT, smid);
	}

	va_list args;
	va_start(args, fmt);

	char buffer[32] = {0}; // Enough for 64-bit hex and dec

	bool acceptingFormat = false;
	for(; *fmt; ++fmt) {
		if(acceptingFormat) {
			switch(*fmt) {
			case 's':
				_writes(va_arg(args, const char*));
				break;
			case 'c':
				_writec((char)va_arg(args, int));
				break;
			case 'x':
				_writes(std::_hexToStr(va_arg(args, uint64_t), buffer));
				break;
			case 'd': // fallthrough (for now at least)
			case 'u':
				_writes(std::_u64ToStr(va_arg(args, uint64_t), buffer));
				break;
			}

			acceptingFormat = false;
		} else if(*fmt == '%') {
			acceptingFormat = true;
		} else {
			_writec(*fmt);
		}
	}

	return 0;
}
