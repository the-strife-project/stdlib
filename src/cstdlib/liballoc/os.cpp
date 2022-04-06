#include "liballoc_1_1.h"
#include "../PageAlloc/PageAlloc.hpp"

// OS-dependent things for liballoc

int liballoc_lock() { return 0; } // TODO
int liballoc_unlock() { return 0; }

void* liballoc_alloc(size_t npages) { return __allocp(npages); }

int liballoc_free(void* ptr, size_t npages) {
	__freep(ptr, npages);
	return 0;
}

// C++
void* operator new(size_t sz) { return malloc(sz); }
void* operator new[](size_t sz) { return malloc(sz); }
void operator delete(void* p) { free(p); }
void operator delete[](void* p) { free(p); }
