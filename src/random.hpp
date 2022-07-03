#ifndef _STDLIB_RANDOM_HPP
#define _STDLIB_RANDOM_HPP

namespace std {
	inline uint64_t rand64() {
		// TODO: USE KERNEL CSPRNG!
		uint64_t ret;
		asm volatile("rdrand %0"
					 : "=a" (ret)
					 :: "cc", "memory");
		return ret;
	}
};

#endif
