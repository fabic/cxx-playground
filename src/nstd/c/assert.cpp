#include <nstd/c/assert.h>
#include <nstd/Process.hpp>

using namespace nstd;

/**
 * Musl-libc `src/exit/assert.c`
 *
 * TODO: See src/exit/abort.c, which raises SIGABRT and SIGKILL (then loops forever).
 * TODO: See src/exit/exit.c and src/exit/_Exit.c
 */
void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
	//fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
	//fflush(NULL);
  if (expr != nullptr)
    Process::write(expr);

  if (file != nullptr)
    Process::write(file);

  if (func != nullptr)
    Process::write(func);

  Process::abort();
}
