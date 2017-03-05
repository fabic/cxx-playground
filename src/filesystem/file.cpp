#include "filesystem/file.hpp"
#include "util/logging.hpp"

namespace dude {
namespace fs {

  // ctor
  File::File(xfs::path fileName) : _fileName(fileName) {}


  // dtor
  File::~File()
  {
    if (buffer_)
      delete[] buffer_;

    if (_fs.is_open())
      _fs.close();
  }


  std::uintmax_t
    File::size()
    {
      return xfs::file_size(_fileName);
    }


  File::self
    File::read()
    {
      auto fileSize = size();

      logtrace << "Reading whole content of file: " << _fileName
               << ", size: " << fileSize;

      // We do close the file, and re-open it for reading [absurd?].
      if (_fs.is_open()) {
        _fs.close();
        logtrace << "Closed previously opened file.";
      }

      // Try open.
      _fs.open(_fileName.c_str(), std::ios_base::in);
      if (_fs.fail())
        throw dude::ex::yet_undefined_exception("Failed to open file.");

      bufferAllocate(fileSize + 1);

      _fs.read(buffer_, fileSize);

      auto readCount = _fs.gcount();
      if (readCount != fileSize) {
        logwarn << "Beware: read less bytes than the file size: "
                << readCount << " <> " << fileSize;
      }

      // Try to read one more character so as to reach the EOF stream state.
      auto ch = _fs.get();
      if (! _fs.eof()) {
        logwarn << "Didn't reach the end of file ?";
        _fs.putback(ch);
      }

      logtrace << "Finished reading whole content of file: " << _fileName;

      return *this;
    }


  File::self
    File::bufferAllocate(std::uintmax_t size)
    {
      if (buffer_ != nullptr)
        delete[] buffer_;

      buffer_ = new char_type [size];

      return *this;
    }

} // fs ns.
} // dude ns.
