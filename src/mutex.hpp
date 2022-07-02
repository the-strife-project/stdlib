#ifndef _STDLIB_MUTEX_HPP
#define _STDLIB_MUTEX_HPP

namespace std {
	// This is a spinlock for now, TODO
	class mutex {
	private:
		volatile size_t x = 0;

	public:
		inline void acquire() {
			asm volatile("0:\n"
						 "lock btsq $0, %0\n"
						 "jnc 2f\n"

						 "1:\n"
						 "pause\n"
						 "testq $1, %0\n"
						 "jnc 1b\n"
						 "jmp 0b\n"

						 "2:"
						 : "+m" (x)
						 :: "memory", "cc");
		}

		inline void release() { x &= ~1; }

		inline void lock() { acquire(); }
		inline void unlock() { release(); }
	};
};

#endif
