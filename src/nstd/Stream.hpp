#ifndef _NSTD_KERNEL_STREAM_H
#define _NSTD_KERNEL_STREAM_H

#include <Syscall.h>

namespace kernel {


  using nstd::ssize_t;
  using nstd::size_t;

  class Stream {
    public:
      static ssize_t read(int fd, void *buffer, size_t count);
      static ssize_t write(int fd, void *buffer, size_t count);
  };

  ///
  ssize_t Stream::read(int fd, void *buffer, size_t count)
  {
    long buf = reinterpret_cast<long>( buffer );

    return Syscall::syscall3(Syscall::SYS_read, fd, buf, count);
  }

  ///
  ssize_t Stream::write(int fd, void *buffer, size_t count)
  {
    long buf = reinterpret_cast<long>( buffer );

    return Syscall::syscall3(Syscall::SYS_write, fd, buf, count);
  }

} // kernel ns.

#endif // _NSTD_KERNEL_STREAM_H
