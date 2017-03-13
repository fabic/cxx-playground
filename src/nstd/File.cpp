#include <nstd/File.hpp>
#include <nstd/String.hpp>
#include <nstd/c/assert.h>

namespace nstd {

  File::File(int fd)
    : _fd(fd)
  {}


  File::File(const char *pathName, int flags)
    : File( open(pathName, flags) )
  {
  }


  File // static btw.
    File::open(const char *pathName, int flags, int mode)
    {
      int fd = Stream::open(pathName, flags, mode);
      return File( fd );
    }


  ssize_t
    File::write(const char *str)
    {
      assert(str != nullptr && "Null string pointer !");
      size_t len = String::strlen( str );
      return Stream::write(_fd, str, len);
    }


  ssize_t
    File::writeln(const char *str)
    {
      return write(str) + write("\n");
    }


} // nstd ns.
