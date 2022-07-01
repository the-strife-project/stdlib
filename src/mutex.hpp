#ifndef _STDLIB_MUTEX_HPP
#define _STDLIB_MUTEX_HPP

namespace std {
	class mutex {
	public:
		// TODO!

		inline void acquire() {}
		inline void release() {}

		inline void lock() { acquire(); }
		inline void unlock() { release(); }
	};
};

#endif
