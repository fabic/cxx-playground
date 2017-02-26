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
    return kernel::Stream::write(fd, str, len);
  }

  ssize_t Process::writeln(const char *str, int fd)
  {
    return write(str, fd) + write("\n", fd);
  }

} // nstd ns.

