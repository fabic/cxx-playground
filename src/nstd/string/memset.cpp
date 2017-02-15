
#include <types.h>

/**
 * TODO: impl. memset
 */
extern "C"
void* memset(void* dest, int c, nstd::size_t n)
{
  auto s = reinterpret_cast<unsigned char *>( dest );

	for (; n; n--, s++) *s = c;

	return dest;
}
