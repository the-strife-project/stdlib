#ifndef _STDLIB_ALGORITHM_HPP
#define _STDLIB_ALGORITHM_HPP

namespace std {
	template<typename T>
	const T& min(const T& a, const T& b) {
		return a < b ? a : b;
	}
}

#endif
