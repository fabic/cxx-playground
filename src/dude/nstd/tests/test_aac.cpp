
#include <nstd/kernel/linux/Process.hpp>
#include <nstd/c/assert.h>

using namespace kernel;


//void _init() __attribute__((weak));
//void _fini() __attribute__((weak));
extern "C" void _start_x() __attribute__((noreturn));

int main(int argc, char *argv[], char *env[]);


/** Borrowed from musl-libc: crt/x86_64/crt1.s
 *
 * Written 2011 Nicholas J. Kain, released as Public Domain
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
 *   | Start address : | Description :
 *   |-----------------+-----------------------------------------------------
 *   |                 | (^ high addresses)
 *   |                 | Information block, including argument strings,
 *   |                 | environment strings, auxiliary information, etc...
 *   | <UNSPECIFIED>   |
 *   |                 | NULL (end of auxiliary vector entries).
 *   |                 | Auxiliary vector entries ...
 *   |                 | NULL (end of environment pointers).
 *   |                 | Environment pointers ...
 *   | %rsp+8 + 8*argc | NULL marker (end of argv[])
 *   | ...             |
 *   | %rsp+8          | char *argv[] | Argument pointers ...
 *   | %rsp            | argc         | Argument count
 *   |                 | <PROCESS STACK TOP>
 *   |                 | (low addresses)
 */
asm
(
  ".weak _init    \n"
  ".weak _fini    \n"

  ".text            \n"
  ".global _start_x \n"

  "_start_x:              \n"
  "  xor %rbp, %rbp       \n" /* rbp:undefined -> mark as zero 0 (abi) */
  "    mov %rdx, %r9      \n" /* 6th arg: ptr to register with atexit() */
  "    pop %rsi           \n" /* 2nd arg: argc */
  "    mov %rsp, %rdx     \n" /* 3rd arg: argv */
  "  andq $-16, %rsp      \n" /* align stack pointer */
  "    mov $_fini, %r8    \n" /* 5th arg: fini/dtors function */
  "    mov $_init, %rcx   \n" /* 4th arg: init/ctors function */
  "    mov $main, %rdi    \n" /* 1st arg: application entry ip */
  "  call _start_x_c        \n"/* musl init will run the program */

  "nop\nnop\nnop\nnop\n"
);


/**
 * musl-libc/crt/crt1.c
 */
extern "C" // _Noreturn
void _start_x_c(
    int (*main_)(),
    int argc, char **argv,
    void (*init_)(),
    void (*fini_)(),
    void(*atexit_wtf)()
    )
{
  assert( argv[argc] == nullptr );

  char **envp = &argv[ argc + 1 ];

  int ec = main(argc, argv, envp);

  Process::exit(100 + ec);
}


/**
 * MAIN !
 */
int main(int argc, char *argv[], char *env[])
{
    return argc;
}

