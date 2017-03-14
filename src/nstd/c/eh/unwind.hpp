#ifndef NSTD_C_EH_UNWIND_H
#define NSTD_C_EH_UNWIND_H

#include <nstd/types.hpp>

namespace nstd {

  enum _Unwind_Reason_Code
  {
    _URC_NO_REASON                = 0,
    _URC_FOREIGN_EXCEPTION_CAUGHT = 1,
    _URC_FATAL_PHASE2_ERROR       = 2,
    _URC_FATAL_PHASE1_ERROR       = 3,
    _URC_NORMAL_STOP              = 4,
    _URC_END_OF_STACK             = 5,
    _URC_HANDLER_FOUND            = 6,
    _URC_INSTALL_CONTEXT          = 7,
    _URC_CONTINUE_UNWIND          = 8
  } ;


  struct _Unwind_Exception; // forward decl.

  typedef void (*_Unwind_Exception_Cleanup_Fn)( _Unwind_Reason_Code,
                                                struct _Unwind_Exception * );


  /* The C++ ABI requires exception_class, private_1, and private_2 to
     be of type uint64 and the entire structure to be
     double-word-aligned. */
  struct _Unwind_Exception
  {
    uint64_t exception_class;
    _Unwind_Exception_Cleanup_Fn exception_cleanup;
    uint64_t private_1;
    uint64_t private_2;
  } __attribute__((__aligned__));

} // nstd ns.

/*
 * * `clang/lib/Headers/unwind.h`
 * * `libunwind/include/unwind.h`
 */

extern "C" {


  void _Unwind_Resume (struct _Unwind_Exception *);

} // extern "C" //

#endif // NSTD_C_EH_UNWIND_H
