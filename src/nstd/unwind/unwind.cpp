#include <nstd/unwind/unwind.hpp>

namespace nstd {
namespace unwind {

  extern "C" {

    // ~/dev/llvm-clang/misc/libunwind/src/unwind/Resume.c
    void
      _Unwind_Resume (struct _Unwind_Exception *exception_object)
      {
      }

  } // extern "C" //

} // unwind ns.
} // nstd ns.
