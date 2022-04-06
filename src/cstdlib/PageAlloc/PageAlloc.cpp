#include "PageAlloc.hpp"
#include <syscalls>

#define FREE false
#define USED true

// Max PAGE_SIZE*8 pages (128M). Can be done better. TODO.
static size_t* bitmap = nullptr;
static size_t first = 0;
static size_t res = 1;

inline size_t bitmapByte(size_t i) { return i / (sizeof(size_t) * 8); }
inline size_t bitmapOff(size_t i)  { return i % (sizeof(size_t) * 8); }

inline bool bitmapGet(size_t i) {
	return bitmap[bitmapByte(i)] & (1ull << bitmapOff(i));
}
inline void bitmapSet(size_t i, bool v) {
	if(v)
		bitmap[bitmapByte(i)] |= 1ull << bitmapOff(i);
	else
		bitmap[bitmapByte(i)] &= ~(1ull << bitmapOff(i));
}

inline static void bitmapMultiSet(size_t begin, size_t n, bool v) {
	for(size_t i=0; i<n; ++i) {
		// Consistency check
		if(bitmapGet(begin+i) == v)
			*(uint64_t*)0x11111 = 0;

		bitmapSet(begin+i, v);
	}
}

void* __allocp(size_t want) {
	if(!bitmap) {
		bitmap = (size_t*)std::moreHeap();
		first = std::moreHeap();
	}

	// Probe
	size_t have = 0;
	size_t ret = first;
	for(size_t i=0; i<(PAGE_SIZE*8); ++i) {
		// Is this page allocated?
		if(i >= res) {
			// No. Need to get some more.
			// Might allocate unfreeable memory, but only once.
			std::moreHeap(want - have);
			res += want - have;
			// Mark as used
			bitmapMultiSet((ret - first) / PAGE_SIZE, want, USED);
			return (void*)ret;
		}

		// Used?
		if(bitmapGet(i)) {
			// Yes. Tough luck.
			have = 0;
			ret = first + (i+1)*PAGE_SIZE;
		} else {
			// No. Promising.
			if(++have == want) {
				// Got it. Mark the pages as used. have=want.
				bitmapMultiSet((ret - first) / PAGE_SIZE, want, USED);
				return (void*)ret;
			}
		}
	}

	// Structural limit
	return nullptr;
}

void __freep(void* page, size_t npages) {
	size_t init = (size_t)page - (size_t)first;
	init /= PAGE_SIZE;
	bitmapMultiSet(init, npages, FREE);
}
