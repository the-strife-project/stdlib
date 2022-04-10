#ifndef _STDLIB_ALGORITHM_HPP
#define _STDLIB_ALGORITHM_HPP

#include <utility>

namespace std {
	template<typename T>
	const T& min(const T& a, const T& b) {
		return a < b ? a : b;
	}

	template<typename T>
	void swap(T& a, T& b) {
		T aux = a;
		a = std::move(b);
		b = std::move(aux);
	}
}

#endif
