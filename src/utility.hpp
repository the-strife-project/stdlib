#ifndef _STDLIB_UTILITY_HPP
#define _STDLIB_UTILITY_HPP

namespace std {
	template<typename T> T&& move(T& x) { return static_cast<T&&>(x); }
}

#endif
