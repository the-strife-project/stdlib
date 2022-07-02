#include "liballoc_1_1.h"
#include "../PageAlloc/PageAlloc.hpp"
#include <mutex>

// OS-dependent things for liballoc

static std::mutex lock;

int liballoc_lock() { lock.acquire(); return 0; }
int liballoc_unlock() { lock.release(); return 0; }

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
