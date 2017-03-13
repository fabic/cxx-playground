#ifndef NSTD_FILE_H
#define NSTD_FILE_H

#include <nstd/kernel/linux/Stream.hpp>

namespace nstd {

  using kernel::Stream;

  /**
   * Wrapper class around a file descriptor.
   */
  class File
  {
    public:
      /// File descriptor integral type.
      using descriptor_t = int ;
    protected:
      /// The file descriptor identifier.
      descriptor_t  _fd;
    public:
      /// Ctor
      explicit File(int fd = -1);

      File(const char *pathName,
           int flags = Stream::OpenFlags::RDONLY);

      descriptor_t
        open(
            const char *pathName,
            int flags = Stream::OpenFlags::RDONLY,
            int mode = 0
          );

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
