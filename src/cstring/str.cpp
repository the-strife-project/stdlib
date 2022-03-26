#include "cstring.hpp"

size_t strlen(const char* str) {
	// Slow, sure, but this shouldn't be used often any way.
	// Pro tip: don't use C strings
	size_t ret = 0;
	while(*str++)
		++ret;
	return ret;
}

/*char* strcpy(const char* str) {
	Requires allocator lmao
}*/
