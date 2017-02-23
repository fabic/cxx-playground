#ifndef NSTD_KERNEL_SIGNAL_H
#define NSTD_KERNEL_SIGNAL_H

#include <nstd/Process.h>

namespace kernel {

  class Signal {
  public:

    /** Sends the signal `sig` to the thread with the thread ID `tid`
     * in the thread group `tgid`.
     *
     *
     * * `tgkill()` is Linux-specific and should not be used in programs
     *   that are intended to be portable.
     * * See the description of `CLONE_THREAD` in `clone()`
     *   for an explanation of thread groups.
     * * `int tgkill(int tgid, int tid, int sig);`
     * * <http://man7.org/linux/man-pages/man2/tgkill.2.html>
     *
     * ## Errors :
     *
     * * `EINVAL` An invalid thread ID, thread group ID, or signal was specified.
     * * `EPERM`  Permission denied.  For the required permissions, see kill(2).
     * * `ESRCH`  No  process  with	 the  specified	 thread	 ID (and thread group ID) exists.
     *
     * \return On success, zero is returned.  ~~On error, -1 is returned,
     * and `errno` is set appropriately.~~
     */
    static inline long tgkill(pid_t tgid, pid_t tid, long sig) {
      return Syscall::syscall3(Syscall::SYS_tgkill, tgid, tid, sig);
    }

  };

} // kernel ns.

#endif // NSTD_KERNEL_SIGNAL_H
