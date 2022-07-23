#include "random.hpp"

static inline uint64_t rol64(uint64_t x, size_t k) {
	return (x << k) | (x >> (64 - k));
}

static uint64_t xoshiro256ss(uint64_t* s) {
	uint64_t ret = rol64(s[1] * 5, 7) * 9;
	uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;
	s[3] ^= rol64(s[3], 45);

	return ret;
}

static uint64_t splitmix64(uint64_t& s) {
	uint64_t result = (s += 0x9E3779B97f4A7C15);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}



std::BadRNG::BadRNG(uint64_t seed_) {
	seed(seed_);
}

void std::BadRNG::seed(uint64_t seed_) {
	for(size_t i=0; i<4; ++i)
		state.s[i] = orig.s[i] = splitmix64(seed_);
}

void std::BadRNG::reset() {
	for(size_t i=0; i<4; ++i)
		state.s[i] = orig.s[i];
}

uint64_t std::BadRNG::next() {
	return xoshiro256ss(state.s);
}
