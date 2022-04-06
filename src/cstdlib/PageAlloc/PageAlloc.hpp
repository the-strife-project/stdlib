#ifndef PAGE_ALLOC_HPP
#define PAGE_ALLOC_HPP

#include <types>

// This is a very simple bitmap allocator

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

void* __allocp(size_t npages=1);
void __freep(void*, size_t npages=1);

#endif
