
//#include <nstd/kernel/linux/Process.hpp>
#include <nstd/Process.hpp>
#include <nstd/c/assert.h>

using nstd::Process;

void _init() __attribute__((weak));
void _fini() __attribute__((weak));

extern "C"
void _start_c(
    long *stack_top_ptr,
    int argc, char **argv,
    void(*atexit_thing_wtf)()
  ) __attribute__((noreturn));

extern "C" int main(int argc, char *argv[], char *env[]);


/** `_start()` program low-level entry point, invoques _start_c(),
 * which in turn invoques main().
 *
 * * Borrowed from musl-libc: `crt/x86_64/crt1.s`
 * * Written 2011 Nicholas J. Kain, released as Public Domain
 * * See Musl-libc commit c5e34dabbb47d8e97a4deccbb421e0cd93c0094b (2013)
 *   “new mostly-C crt1 implementation” ;
 * * and commit 6fef8cafbd0f6f185897bc87feb1ff66e2e204e1 (2015).
 *   “remove hand-written crt1.s and Scrt1.s files for all archs”
 *
 * From \href http://refspecs.linuxbase.org/elf/x86-64-abi-0.99.pdf (page 29, 30):
 *
 * %rsp : The stack pointer holds the address of the byte with lowest address
 *        which is part of the stack. It is guaranteed to be 16-byte aligned
 *        at process entry.
 * %rbp : The content of this register is unspecified at process initialization
 *        time, but the user code should mark the deepest stack frame by setting
 *        the frame pointer to zero.
 * %rdx : A function pointer that the application should register with atexit().
 *
 * Initial process stack :
 *
 *     +-----------------+-----------------------------------------------------
 *     | Start address : | Description :
 *     |-----------------+-----------------------------------------------------
 *     |                 | (^ high addresses)
 *     |                 | Information block, including argument strings,
 *     |                 | environment strings, auxiliary information, etc...
 *     | <UNSPECIFIED>   |
 *     |                 | NULL (end of auxiliary vector entries).
 *     |                 | Auxiliary vector entries ...
 *     |                 | NULL (end of environment pointers).
 *     |                 | Environment pointers ...
 *     | %rsp+8 + 8*argc | NULL marker (end of argv[])
 *     | ...             |
 *     | %rsp+8          | char *argv[] | Argument pointers ...
 *     | %rsp            | argc         | Argument count
 *     |                 | <PROCESS STACK TOP>
 *     |                 | (low addresses)
 *     +-----------------+-----------------------------------------------------
 */
#if 0 // This was the version for statically linked programs.
asm
(
  ".weak _init    \n"
  ".weak _fini    \n"

  ".text                  \n"
  ".global _start         \n"
  ".p2align 4, 0x90       \n"
  ".type _start,@function \n"

  "_start:                \n"
  "  xor %rbp, %rbp       \n" /* rbp:undefined -> mark as zero 0 (abi) */
  "    mov %rdx, %r9      \n" /* 6th arg: ptr to register with atexit() */
  "    pop %rsi           \n" /* 2nd arg: argc */
  "    mov %rsp, %rdx     \n" /* 3rd arg: argv */
  "  andq $-16, %rsp      \n" /* align stack pointer */
  "    mov $_fini, %r8    \n" /* 5th arg: fini/dtors function */
  "    mov $_init, %rcx   \n" /* 4th arg: init/ctors function */
  "    mov $main, %rdi    \n" /* 1st arg: application entry ip */
  "  call _start_c        \n"/* musl init will run the program */
  "  hlt                  \n"
);
#else // rdi, rsi, rdx, rcx, r8, r9.
asm
(
  ".text                  \n"
  ".global _start         \n"
  ".p2align 8, 0x90       \n" // admittedly useless -_- but I'm a nood for now.
  ".type _start,@function \n"

  "_start:                \n"
  "  xor %rbp, %rbp       \n" /* rbp:undefined -> mark as zero 0 (abi) */
  "  mov %rsp, %rdi       \n" /* 1st arg: pointer to top of stack. */
  "    mov %rdx, %rcx     \n" /* 4th arg: ptr to register with atexit() */
  "    mov  (%rsp), %rsi  \n" /* 2nd arg: argc */
  "    mov 8(%rsp), %rdx  \n" /* 3rd arg: argv */
  "  andq $-16, %rsp      \n" /* align stack pointer */
  "  call _start_c        \n"/* musl init will run the program */

  "  hlt                  \n"

  "nop\nnop\nnop\nnop\n" /* padding, alignment */
);
#endif // 0

/**
 * `musl-libc/crt/crt1.c`
 *
 * TODO: See src/env/__libc_start_main.c
 *
 * \param atexit_wtf TODO: find out what that is.
 */
extern "C" // _Noreturn
void _start_c(
    long *stack_top_ptr,
    int argc, char **argv,
    void(*atexit_thing_wtf)()
    )
{
  /* For information: argc & argv may simply have been inferred here,
   * instead of being setup by the _start assembly. */
  {
    int    _argc = static_cast<int>( *stack_top_ptr );
    char **_argv = reinterpret_cast<char**>( stack_top_ptr + 1 );
  }

  assert( argv[argc] == nullptr );

  char **envp = &argv[ argc + 1 ];

  int ec = main(argc, argv, envp);

  // TODO: See src/exit/atexit.c: __cxa_atexit(), __cxa_finalize() and atexit().

  Process::exit(100 + ec);

  // Unreachable; prevents Clang from generating a warning
  Process::abort();
}

extern "C"
void _bootstrap_stub(
    decltype(_init) init,
    decltype(_fini) fini,
    void(*atexit_thing_wtf)()
  )
{
}
