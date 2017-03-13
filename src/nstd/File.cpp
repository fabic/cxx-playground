#include <nstd/File.hpp>
#include <nstd/String.hpp>
#include <nstd/c/assert.h>
#include <nstd/kernel/linux/errno.hpp>

namespace nstd {


  File::File(int fd)
    : _fd(fd)
  {}


  File::File(const char *pathName, int flags)
    : File()
      // ^ invoque the default ctor which will set `_fd` to -1.
  {
    open(pathName, flags);
  }


  File::descriptor_t
    File::open(const char *pathName, int flags, int mode)
    {
      assert(_fd < 0);

      auto fd = Stream::open(pathName, flags, mode);

      debug_if(fd < 0, kernel::error_description(_fd));

      _fd = fd;

      return _fd;
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
