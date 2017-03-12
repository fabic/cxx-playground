
#include <nstd/Process.hpp>

using namespace nstd;

/**
 * MAIN !
 */
int main(int argc, const char *argv[], const char *env[])
{
  Process::write("Hello! I'm ");
  Process::writeln(argv[0]);

  for(int i = 1; i < argc; i++)
  {
    auto pathName = argv[i];
    Process::write("Arg.: ");
    Process::writeln(pathName);
    long fd = Process::open(pathName, Process::OpenFlags::READ_ONLY);
  }

  Process::writeln("Bye.");

  return 111;
}

