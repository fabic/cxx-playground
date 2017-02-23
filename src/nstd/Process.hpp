#ifndef NSTD_PROCESS_H
#define NSTD_PROCESS_H

#include <nstd/kernel/linux/Process.hpp>
#include <nstd/kernel/linux/Signal.hpp>

namespace nstd {

  class Process : protected kernel::Process,
                  protected kernel::Signal {
  public:
    /**
     * * `lib/musl/src/exit/abort.c`
     * * <http://man7.org/linux/man-pages/man3/abort.3.html>
     * * <http://en.cppreference.com/w/cpp/utility/program/abort>
     *
     * * TODO: read abort(3): we need to unblock the ABRT signal 1st
     *   before triggering it; _and_ in case it was caught or ignored,
     *   we should restore the "default disposition" for SIGABRT and
     *   attempt to raise the signal a 2nd time.
     *
     * * This function never returns -- _warning:_ an infinite loop lies there.
     */
    static void abort() __attribute__((noreturn));

  };

} // nstd ns.

#endif // NSTD_PROCESS_H
