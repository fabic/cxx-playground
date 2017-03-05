#ifndef _DUDE_FILESYSTEM_FILE_H
#define _DUDE_FILESYSTEM_FILE_H

#include <fstream>
//#include <experimental/filesystem>
#include <boost/filesystem.hpp>
#include <cstdint>
# include "util/exceptions.hpp"

namespace dude {
namespace fs {

  //namespace xfs = std::experimental::filesystem;
  namespace xfs = boost::filesystem;

  /**
   */
  class File {
    public:
      using self = File& ;
      using char_type = std::fstream::char_type;
    protected:
      xfs::path     _fileName;
      std::fstream  _fs;
      //
      char_type *   buffer_ = nullptr;
      std::size_t   _bufferSize;
    public:
      explicit File(xfs::path fileName);
      virtual ~File();
      std::uintmax_t size();
      self read();
    protected:
      self bufferAllocate(std::uintmax_t size);
  };

} // fs ns.
} // dude ns.

#endif // _DUDE_FILESYSTEM_FILE_H
