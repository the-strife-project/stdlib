#ifndef _STDLIB_MUTEX_HPP
#define _STDLIB_MUTEX_HPP

#include <syscalls>
#include <queue>

namespace std {
	// Regular "test, test and set" spinlock
	class Spinlock {
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
		inline bool isAcquired() { return x; }
	};



	class Semaphore {
	private:
		Spinlock x;
		size_t slots;
		size_t available;
		std::queue<std::PID> q;

	public:
		// Default is mutex
		Semaphore(size_t s_=1) : slots(s_), available(s_) {}

		inline void acquire() {
			if(!slots) {
				// Global initializer failed, quietly patch (TODO)
				slots = available = 1;
			}

			x.acquire();
			if(!available) {
				q.push(std::getOrigPID());
				x.release();
				std::lockme();
			} else {
				--available;
				x.release();
			}
		}

		inline void release() {
			x.acquire();
			if(q.size()) {
				std::PID pid = q.front();
				q.pop();
				std::wake(pid);
			} else {
				if(available < slots)
					++available;
			}
			x.release();
		}

		inline void lock() { acquire(); }
		inline void unlock() { release(); }
		inline size_t waiting() {
			x.acquire();
			auto ret = q.size();
			x.release();
			return ret;
		}
		inline bool isAcquired() {
			x.acquire();
			auto ret = available < slots;
			x.release();
			return ret;
		}
	};

	using mutex = Semaphore;



	// Inverse semaphore, solves the known «Cigarette smokers problem»
	// as well as the equivalent «Readers–writers problem»
	class Smokers {
	private:
		mutex m;
		Spinlock x;
		size_t count = 0;

	public:
		Smokers() { m.acquire(); }

		inline void give() {
			x.acquire();
			++count;
			if(m.waiting())
				m.release();
			x.release();
		}

		inline void take() {
			x.acquire();
			if(!count) {
				x.release();
				m.acquire(); // Wait for it
				x.acquire();
			}
			--count;
			x.release();
		}

		inline size_t _getCount() {
			x.acquire();
			auto ret = count;
			x.release();
			return ret;
		}
	};
};

#endif
