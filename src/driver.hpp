#ifndef _STDLIB_DRIVER_HPP
#define _STDLIB_DRIVER_HPP

#include <types>

namespace std {
	inline uint8_t in8(uint16_t port) {
		uint8_t ret;
		asm volatile("inb %1, %0"
					 : "=a" (ret)
					 : "dN" (port));
		return ret;
	}

	inline void out8(uint16_t port, uint8_t val) {
		asm volatile("outb %1, %0"
					 :
					 : "dN" (port),
					   "a" (val));
	}



	inline uint16_t in16(uint16_t port) {
		uint16_t ret;
		asm volatile("inw %1, %0"
					 : "=a" (ret)
					 : "dN" (port));
		return ret;
	}

	inline void out16(uint16_t port, uint16_t val) {
		asm volatile("outw %1, %0"
					 :
					 : "dN" (port),
					   "a" (val));
	}



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
