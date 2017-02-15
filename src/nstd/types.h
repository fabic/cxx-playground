#ifndef __NSTD_TYPES_H
#define __NSTD_TYPES_H

namespace nstd {

  typedef unsigned long* uintptr_t;
  typedef unsigned long size_t;

  static_assert(sizeof(size_t) == sizeof(void*),
      "Type size_t should have the same byte-count as a pointer.");
}

#endif // __NSTD_TYPES_H

