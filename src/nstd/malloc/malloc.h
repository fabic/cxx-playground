#ifndef __NSTD_MALLOC_H
#define __NSTD_MALLOC_H

#include <types.h>

// todo: throw bad_alloc.

void* operator new(nstd::size_t sz);

void* operator new[](nstd::size_t sz);

#endif //__NSTD_MALLOC_H
