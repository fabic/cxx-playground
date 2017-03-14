#include <nstd/eh/cxa.hpp>
#include <nstd/c/assert.h>

namespace nstd {
namespace cxxabi {

  extern "C" {

    void *
      __cxa_begin_catch(void * unwind_arg) noexcept
      {
        return nullptr;
      }

  } // extern "C" //

} // cxxabi ns.
} // nstd ns.

