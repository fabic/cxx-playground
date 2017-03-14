#ifndef NSTD_C_EH_CXA_H
#define NSTD_C_EH_CXA_H

#include <nstd/unwind/unwind.hpp>

/*
 * * `~/dev/llvm-clang/llvm-clang/libcxxabi/src/cxa_exception.cpp`
 */
namespace nstd {
namespace cxxabi {

  extern "C" {

    void * __cxa_begin_catch(void * unwind_arg) noexcept;

  } // extern "C" //

} // cxxabi ns.
} // nstd ns.

#endif // NSTD_C_EH_CXA_H
