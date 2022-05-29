#include <cstdio>

static char* changeSize(char* buffer, size_t fixedSize, size_t maxlen) {
	size_t begin = 0;
	if(!fixedSize) {
		while(begin < maxlen-1 && buffer[begin] == '0')
			++begin;
	} else {
		begin = maxlen - fixedSize;
	}

	return buffer + begin;
}

static const char HEX_CHARSET[] = "0123456789ABCDEF";

char* std::_hexToStr(uint64_t x, char* buffer, size_t fixedSize) {
	size_t sz = 16;
	buffer[sz] = 0;
	while(sz--) {
		buffer[sz] = HEX_CHARSET[x & 0x0F];
		x >>= 4;
	}

	return changeSize(buffer, fixedSize, 16);
}

static const char DEC_CHARSET[] = "0123456789";

char* std::_u64ToStr(uint64_t x, char* buffer, size_t fixedSize) {
	size_t sz = 20;
	buffer[sz] = 0;
	while(sz--) {
		buffer[sz] = DEC_CHARSET[x % 10];
		x /= 10;
	}

	return changeSize(buffer, fixedSize, 20); // len(str(1 << 64))
}
