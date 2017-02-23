#include <nstd/Process.hpp>

namespace nstd {

  void Process::abort()
  {
    pid_t tgid = getpid();
    pid_t  tid = gettid();

    long ret = tgkill(tgid, tid, SIGABRT);

    if (ret != 0)
      ; // todo: err. condition ?

    while(true) {} // noreturn (else compiler complains).
  }

} // nstd ns.

