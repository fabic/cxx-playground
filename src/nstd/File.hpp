#ifndef NSTD_FILE_H
#define NSTD_FILE_H

#include <nstd/kernel/linux/Stream.hpp>

namespace nstd {

  using kernel::Stream;

  /**
   */
  class File {
    protected:
      int _fd;
    public:
      explicit File(int fd = -1);

      File(const char *pathName,
           int flags = Stream::OpenFlags::READ_ONLY);

      static File open(
          const char *pathName,
          int flags = Stream::OpenFlags::READ_ONLY,
          int mode = 0);

      ssize_t write(const char *str);
      ssize_t writeln(const char *str);
  };


  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~


  /**
   * “Stream insertion `operator<<()`” that writes a \0 terminated string
   * to file descriptor `file`.
   */
  inline File&
    operator<<(File& file, const char *str)
    {
      file.write( str );
      return file;
    }


} // nstd ns.

#endif // NSTD_FILE_H
