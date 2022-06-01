#ifndef _STDLIB_DRIVER_HPP
#define _STDLIB_DRIVER_HPP

#include <types>

namespace std {
	inline uint32_t in32(uint16_t port) {
		uint32_t ret;
		asm volatile("inl %1, %0"
					 : "=a" (ret)
					 : "dN" (port));
		return ret;
	}

	inline void out32(uint16_t port, uint32_t val) {
		asm volatile("outl %1, %0"
					 :
					 : "dN" (port),
					   "a" (val));
	}
};

#endif
