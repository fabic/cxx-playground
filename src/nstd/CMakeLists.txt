
cmake_minimum_required(VERSION 3.3)

project(NStd)

set (NStd_VERSION_MAJOR 0)
set (NStd_VERSION_MINOR 1)

if(NOT "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "x86")
  message(WARNING "Dude! Ain't no x86 system here ?!")
endif()

if(false)
# https://cmake.org/Wiki/CMake/Assembler
enable_language(ASM-ATT)

if (NOT CMAKE_ASM-ATT_COMPILER_WORKS)
  message(WARNING "Dude! CMake says the assembler does not work -_-")
endif()

endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions()

add_compile_options(
  -std=c++1z
  #-stdlib=libc++
  -fPIC
  -Wall
  -pedantic
  # -Weverything
  #   ^ http://clang.llvm.org/docs/UsersManual.html#enabling-all-diagnostics
  )


add_compile_options(
  -fcolor-diagnostics
  -fdiagnostics-show-category=name
  -fdiagnostics-show-template-tree
  #   ^ http://clang.llvm.org/docs/UsersManual.html#formatting-of-diagnostics

  # -fvisibility=hidden
  #   ^ http://clang.llvm.org/docs/LTOVisibility.html
  #     See Boost.DLL about symbol shadowing problem w/ Linux.

  # -fwhole-program-vtables
  #   ^ http://clang.llvm.org/docs/UsersManual.html#cmdoption-fwhole-program-vtables
)

add_compile_options(
  # -O0 -ggdb
  # ^ this is automatically handled by CMake build type thing.

  # So as to get better/meaninful stack traces in error messages :
  -fno-omit-frame-pointer
  # and disable tail call elimination
  -fno-optimize-sibling-calls
  )

add_compile_options(
  # -fvisibility-inlines-hidden
  # -Wwrite-strings
  # -Wcast-qual
  -Wmissing-field-initializers
  -Wcovered-switch-default
  -Wnon-virtual-dtor
  -Wdelete-non-virtual-dtor
  -Woverloaded-virtual
  -fcolor-diagnostics
  # -ffunction-sections
  # -fdata-sections
  # -fno-common
  # -Wno-nested-anon-types
  # -Wl,-allow-shlib-undefined
)

# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ TARGETS ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

add_compile_options( -nostdinc -nostdinc++ )
# NOTE: Clang also has `-nobuiltininc` but complains
#       that is wasn't used during compilation.

# Prevents Clang from issuing a warning :
#   “ISO C++ does not allow 'main' to be used by a program [-Wmain,Semantic Issue]”
set_source_files_properties( c/_start.cpp PROPERTIES COMPILE_FLAGS -Wno-main )

# ~ ~ ~ Das `nstd` lib. ~ ~ ~
add_library( nstd SHARED
    c/_start.cpp
    c/memset.cpp
    c/assert.cpp
    malloc/malloc.cpp
    String/length.cpp
    Process.cpp
  )
set_target_properties( nstd PROPERTIES LINK_FLAGS -nostdlib)

link_libraries( -nostdlib nstd )
#               ^ manual says it's ok to pass linker flags in this way.
#                 Alternate solution was :
#set_target_properties( test_aac PROPERTIES LINK_FLAGS -nostdlib)

#add_subdirectory(tests)

add_executable( test_aab tests/test_aab.cpp )
add_executable( test_aac tests/test_aac.cpp )
add_executable( test_aad tests/test_aad.cpp )
add_executable( test_aae tests/test_aae.cpp )
add_executable( test_strlen String/test_length.cpp)
add_executable( test_aaf tests/test_aaf.cpp )

# ~ ~ ~ * ~ ~ ~

# Old thing that is.
if (false)
  add_executable( aaa tests/test001.cpp
      #../hacks/hack_cxa_throw_exception.cpp
    )
  target_include_directories( aaa PUBLIC )
  target_link_libraries( aaa
      # ${Boost_LIBRARIES}
      # Unwind dl # for hack_cxa_throw_exception.cpp
    )
endif()
