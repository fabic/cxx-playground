
#include <malloc/malloc.h>
#include <Process.h>

using kernel::Process;

/**
 * `[...] undefined reference to 'operator new(unsigned long)'.`
 * See `llvm-clang/libcxx/include/new`
 */
void* operator new(nstd::size_t sz)
{
  nstd::uintptr_t ptr = Process::brk();
  ptr += sz;
  Process::brk(ptr);
  return ptr;
}
