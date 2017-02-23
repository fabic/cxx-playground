#ifndef NSTD_PROCESS_H
#define NSTD_PROCESS_H

#include <nstd/kernel/linux/Process.hpp>

namespace nstd {

  class Process : protected kernel::Process {
  public:
    /**
     * `lib/musl/src/exit/abort.c`
     */
    static void abort() __attribute__((noreturn));

  };



  void Process::abort() {

  }

} // nstd ns.

#endif // NSTD_PROCESS_H
