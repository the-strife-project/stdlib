#ifndef PAGE_ALLOC_HPP
#define PAGE_ALLOC_HPP

#include <types>

// This is a very simple bitmap allocator

void* __allocp(size_t npages=1);
void __freep(void*, size_t npages=1);

#endif
