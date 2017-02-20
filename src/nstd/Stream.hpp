#ifndef _NSTD_KERNEL_STREAM_H
#define _NSTD_KERNEL_STREAM_H

#include <Syscall.h>

namespace kernel {

  using nstd::ssize_t;
  using nstd::size_t;

  /** File/descriptor related syscalls.
   */
  class Stream {
    public:
      static inline ssize_t read(int fd, void *buffer, size_t count);
      static inline ssize_t write(int fd, const void *buffer, size_t count);
      static inline ssize_t write(int fd, const char *str, size_t len);
      static inline int     close(int fd);
      // todo: does the kernel actually returns an 'int' ?? or is it libc ?
  };

  //
  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
  //

  ///
  ssize_t Stream::read(int fd, void *buffer, size_t count)
  {
    long buf = reinterpret_cast<long>( buffer );

    return Syscall::syscall3(Syscall::SYS_read, fd, buf, count);
  }

  ///
  ssize_t Stream::write(int fd, const void *buffer, size_t count)
  {
    long buf = reinterpret_cast<long>( buffer );

    return Syscall::syscall3(Syscall::SYS_write, fd, buf, count);
  }

  ///
  ssize_t Stream::write(int fd, const char *str, size_t len) {
    return Stream::write(fd, reinterpret_cast<const void *>(str), len);
  }

  ///
  int Stream::close(int fd)
  {
    return Syscall::syscall1(Syscall::SYS_close, fd);
  }

} // kernel ns.

#endif // _NSTD_KERNEL_STREAM_H
