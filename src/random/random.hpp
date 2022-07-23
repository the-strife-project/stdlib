#ifndef _STDLIB_RANDOM_HPP
#define _STDLIB_RANDOM_HPP

#include <types>
#include <syscalls>

namespace std {
	// xoshiro256** + splitmix64
	// This is not cryptographically secure, at all
	class BadRNG {
	private:
		struct State { uint64_t s[4]; };

		State orig;
		State state;

	public:
		BadRNG(uint64_t seed_=42);
		void seed(uint64_t seed_);
		void reset();

		uint64_t next();
	};



	inline uint64_t rand64() {
		uint64_t ret;
		std::csprng(&ret, sizeof(uint64_t));
		return ret;
	}
}

#endif
