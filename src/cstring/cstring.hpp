#ifndef _STDLIB_CSTRING_HPP
#define _STDLIB_CSTRING_HPP

#include <types>

// Outside of std :^)
extern "C" void* memcpy(void* dst, void* src, size_t sz);
extern "C" void* memmove(void* dst, void* src, size_t sz);
extern "C" void* memset(void* ptr, char val, size_t sz);

extern "C" size_t strlen(const char* str);
extern "C" char* strcpy(const char* str);

#endif
