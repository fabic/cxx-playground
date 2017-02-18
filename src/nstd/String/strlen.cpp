
#include <String.hpp>

#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

namespace nstd {

  using nstd::size_t;

  /**
   * Borrowed from musl-libc `src/string/strlen.c`
   *
   * * http://stackoverflow.com/a/24805807/643087
   */
  size_t String::length(const char *s)
  {
    constexpr size_t Alignment = sizeof(size_t);

    const char *a = s;
    const size_t *w;

    for (; static_cast<size_t>(s) % Alignment; s++) if (!*s) return s-a;
    for (w = (const void *)s; !HASZERO(*w); w++);
    for (s = (const void *)w; *s; s++);

    return s-a;
  }

} // nstd ns.
