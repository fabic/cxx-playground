#ifndef _NSTD_KERNEL_STREAM_H
#define _NSTD_KERNEL_STREAM_H

#include <nstd/kernel/linux/Syscall.hpp>

namespace kernel {

  using nstd::ssize_t;
  using nstd::size_t;

  /** File/descriptor related syscalls.
   */
  class Stream {
    public:
      static constexpr int STDIN  = 0;
      static constexpr int STDOUT = 1;
      static constexpr int STDERR = 2;
    public:
      static inline ssize_t read(int fd, void *buffer, size_t count);

      template<typename T>
        static inline ssize_t write(int fd, const T* buffer, size_t count);

      static inline int close(int fd);

      // todo: does the kernel actually returns an 'int' ?? or is it libc ?
  };

  //
  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
  //


  ssize_t Stream::read(int fd, void *buffer, size_t count)
  {
    long buf = reinterpret_cast<long>( buffer );

    return Syscall::syscall3(Syscall::SYS_read, fd, buf, count);
  }


  template<typename T>
    ssize_t Stream::write(int fd, const T* buffer, size_t count)
    {
      long buf = reinterpret_cast<long>( buffer );
      long size = sizeof(T) * count;
      return Syscall::syscall3(Syscall::SYS_write, fd, buf, size);
    }


  int Stream::close(int fd)
  {
    return Syscall::syscall1(Syscall::SYS_close, fd);
  }


} // kernel ns.

#endif // _NSTD_KERNEL_STREAM_H
