
#include <nstd/String.hpp>
#include <nstd/Stream.hpp>

/**
 * MAIN !
 */
int main(int argc, char *argv[], char *env[])
{
  using kernel::Stream;
  using nstd::String;

  for(int i = 0; i < argc; i++) {
    auto str = argv[i];
    auto len = String::strlen(str);
    Stream::write(2, str, len);
    Stream::write(2, "\n", 1);
  }

  return 0;
}

