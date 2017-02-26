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


  ssize_t Process::write(const char *str, int fd)
  {
    size_t len = String::strlen( str );

    return kernel::Stream::write(fd, reinterpret_cast<const void *>(str), len);
  }

} // nstd ns.

