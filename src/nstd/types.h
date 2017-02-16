#ifndef __NSTD_TYPES_H
#define __NSTD_TYPES_H

namespace nstd {

  /*
   * \link http://en.cppreference.com/w/cpp/language/types
   */

  // fixme
  typedef unsigned long* uintptr_t;

  typedef unsigned long  size_t;

  /// Signed size_t
  typedef unsigned long ssize_t;

  static_assert(sizeof(void*) == 8, "Pointers should be 64-bits.");

  static_assert(sizeof(size_t) == sizeof(void*),
      "Type size_t should have the same byte-count as a pointer.");

  static_assert(sizeof(long) == sizeof(void*),
      "Integer type 'long' should have the same size as pointers.");

  static_assert(sizeof(int) == sizeof(long)/2,
      "Integer type 'int' should be “half” as 'long'.");
}

#endif // __NSTD_TYPES_H

