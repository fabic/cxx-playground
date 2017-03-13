#ifndef __NSTD_ASSERT_H
#define __NSTD_ASSERT_H

extern "C" {
  void __assert_fail(
      const char *expr,
      const char *file, int line,
      const char *func
    );
}

#ifdef NDEBUG
#  define assert(x) (void)0
#else
#  define assert(x) ((void)((x) || (__assert_fail(#x, __FILE__, __LINE__, __func__),0)))
#endif

extern "C" {
  void __debug_if(
      const char *expr,
      const char *text,
      const char *file, int line,
      const char *func
    );
}

#ifdef NDEBUG
#  define debug_if(x, s) ((void)0)
#else
#  define debug_if(x, s) ((void)((x) && (__debug_if(#x, s, __FILE__, __LINE__, __func__),0)))
#endif

#endif // __NSTD_ASSERT_H
