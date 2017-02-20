#ifndef _NSTD_STRING_H
#define _NSTD_STRING_H

#include <types.h>

namespace nstd {

  using nstd::size_t;

  class String {
    public:
      static size_t strlen(const char *s);

  };

} // nstd ns.

#endif // _NSTD_STRING_H