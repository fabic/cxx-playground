## clong-clang-plugin / 2017-12-16 / fabic.net

cmake_minimum_required(VERSION 3.3)

message(STATUS "~~ Dude, hi! ~~")

project(FabicClong)

set (FabicClong_VERSION_MAJOR 0)
set (FabicClong_VERSION_MINOR 3)

# TODO: Move stuff _out of here (!)_, and specifically: anything that may have
# TODO: side-effects on sub-projects we want to include with add_subdirectory()
# TODO: e.g. vendor/lmdb/

message(STATUS "~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~")
message(STATUS "~~  Dude, hello! This is CMakeLists.txt of '${CMAKE_CURRENT_SOURCE_DIR}'.")
message(STATUS "~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~")

# Add ./cmake/ and ./cmake/modules/ to CMake module search path.
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  message(STATUS "Dude: Added ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ to CMAKE_MODULE_PATH.")
  if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
    message(STATUS "Dude: Also added ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules to CMAKE_MODULE_PATH.")
    include(dude_please_dump_cmake_variables)
    include(list_all_targets)
  endif()
endif()

## Set default build type to Debug if not provided by user.
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel MinSizeDeb."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

message(STATUS "Dude, FYI: CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "Dude, FYI: CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message(STATUS "Dude, FYI: CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

#
## CCACHE
#
if (false)
  find_program(CCACHE_PROGRAM ccache)
  if (CCACHE_PROGRAM)
    set_property( GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}" )
    message(STATUS "Dude! Found ccache, so we're setting CMake global property RULE_LAUNCH_COMPILE to '${CCACHE_PROGRAM}'.")
  endif()
endif()


find_package(LLVM REQUIRED CONFIG)
# find_package(Clang REQUIRED)

# This would typically append for ex. '/usr/lib/cmake/llvm/' to CMake search path.
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

# ./local/{cmake,include,lib}/ : search paths for find_file() and find_library().
if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/local")
  set(DUDE_LOCAL_DIR "${CMAKE_SOURCE_DIR}/local")
  message(STATUS "Dude: Found FHS local/ sub-dir.: ${DUDE_LOCAL_DIR}")

  list(APPEND CMAKE_MODULE_PATH "${DUDE_LOCAL_DIR}/cmake")
  message( STATUS "Updated CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )

  list(APPEND CMAKE_INCLUDE_PATH "${DUDE_LOCAL_DIR}/include")
  message( STATUS "Updated CMAKE_INCLUDE_PATH = ${CMAKE_INCLUDE_PATH}" )
  include_directories( ${DUDE_LOCAL_DIR}/include )

  list(APPEND CMAKE_LIBRARY_PATH "${DUDE_LOCAL_DIR}/lib")
  message( STATUS "Updated CMAKE_LIBRARY_PATH = ${CMAKE_LIBRARY_PATH}" )
  link_directories( "${DUDE_LOCAL_DIR}/lib" )
  # FIXME ? CMake should have inferred this given I didn't define LD_LIBRARY_PATH.
  link_libraries( -Wl,-rpath="${DUDE_LOCAL_DIR}/lib" )

  # TEMP. (!) change default install prefix.
  if ("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local" )
    set(CMAKE_INSTALL_PREFIX "${DUDE_LOCAL_DIR}/local")
    message(STATUS "Dude: overriding CMAKE_INSTALL_PREFIX, now set to '${CMAKE_INSTALL_PREFIX}'.")
  endif()
endif()

# For `add_llvm_loadable_module()` notably.
include(AddLLVM)

# Python is needed in order to produce a PrintFunctionNames.def file
# from the PrintFunctionNames.exports file. Found this somewhere in one of Clang's
# CMake files.
set(Python_ADDITIONAL_VERSIONS 2.7)
include(FindPythonInterp)
if(NOT PYTHONINTERP_FOUND)
  message(WARNING
    "Unable to find Python interpreter, required for builds and testing.
    Please install Python or specify the PYTHON_EXECUTABLE CMake variable.")
else()
  # “If we don't need RTTI or EH, there's no reason to export anything
  #  from the plugin.”
  # FIXME: This breaks the build, linker says the .def file is unrecognizable file format.
  # FIXME: We don't need to export symbols anyway (do we ?)
  if (false)
    if( NOT LLVM_REQUIRES_RTTI )
      if( NOT LLVM_REQUIRES_EH )
        set(LLVM_EXPORTED_SYMBOL_FILE
          ${CMAKE_CURRENT_SOURCE_DIR}/PrintFunctionNames.exports)
        message(STATUS "Dude: Set LLVM_EXPORTED_SYMBOL_FILE = PrintFunctionNames.exports")
      endif()
    endif()
  endif()
endif()

message(STATUS "Dude: Found LLVM version ${LLVM_PACKAGE_VERSION}")
message(STATUS "Dude: Using LLVMConfig.cmake found at: ${LLVM_DIR}")

add_definitions( ${LLVM_DEFINITIONS} )
include_directories( ${LLVM_INCLUDE_DIRS} )

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##

# Source layout :
#  - Either we have an include/ dir. with headers separated from the
#    implementations in some src/ dir.
#  - Or we just have a src/ dir. => in which case we add it as an include (-I)
#    directory.
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
  include_directories( include )
  # include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
elseif (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
  include_directories( src )
  message(STATUS "FYI: Adding src/ as compiler include (-I) path; since there's no include/ subdir. here")
endif()

# Use Clang's libcxx ?
# add_compile_options( -stdlib=libc++ )
# add_compile_options( -stdlib=libstdc++ )

add_compile_options(
  -std=c++1z
  -fPIC
  )

# Warnings
if (true)
  add_compile_options(
    -Wall
    -pedantic
    # -Weverything
    #   ^ http://clang.llvm.org/docs/UsersManual.html#enabling-all-diagnostics
    -Wmissing-field-initializers
    -Wnon-virtual-dtor
    -Wdelete-non-virtual-dtor
    -Woverloaded-virtual
    # Diagnostics
    -fcolor-diagnostics
    -fdiagnostics-show-category=name
    -fdiagnostics-show-template-tree
    )
endif()

# set_property(DIRECTORY PROPERTY EXCLUDE_FROM_ALL ON)
# set(EXCLUDE_FROM_ALL ON)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt")
    add_subdirectory( src )
    message(STATUS "FYI: Added subdirectory src/")
  else()
    message(WARNING "Dude! Found that src/ subdir. of yours, _BUT_ got no src/CMakeLists.txt (!) Skipping...")
  endif()
endif()

# ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

add_subdirectory(experiments)
add_subdirectory(misc)
add_subdirectory(print-fns)

# LMDB : FIXME: don't add_subdirectory(): it inherits stuff that we do here, like compiler flags etc..
if (false)
  find_library(StaticLibLMDB lmdb)
  if ( StaticLibLMDB MATCHES "-NOTFOUND$" )
    message(STATUS "Dude: Couldn't find LMDB's static liblmdb.a")
    add_subdirectory( vendor/lmdb )
  else()
    message(STATUS "Dude: Found LMDB static lib.: ${StaticLibLMDB}")
  endif()
endif()

# TODO: Further setup is needed here, specifically we need to correctly
#       state dependencies so that it is built first.
if (FALSE AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/soci/include/soci/soci.h)
  add_subdirectory( vendor/soci )
endif()

# TODO
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
  )

# Custom target that list executable files.
add_custom_target( list-binaries
  #ALL
  DEPENDS clong-05 # FIXME: last target !
    COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -type d -name CMakeFiles -prune -o -type f -perm -a+x -ls
    COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -type d -name CMakeFiles -prune -o -type f -name '*.a' -ls
    COMMAND echo -e "\\e[32;1;7m  HEY!  \\e[0m"
    COMMAND echo -e "\\e[2m  Dude, have a break away from computer at times!  \\e[0m"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "-- List os executable files (+x) that were built : --"
  VERBATIM USES_TERMINAL
  )


# `cmake --build build --target debug`
ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Run CMake for a Debug build."
  )

# `cmake --build build --target release`
ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Run CMake for a Release build."
  )

# TODO: Custom target that runs built program under LLDB.
#       $ lldb -o 'run' -- ls -lad

dude_list_all_targets(${CMAKE_CURRENT_SOURCE_DIR} _all_library_targets _all_executable_targets _all_other_targets)
message(STATUS "All library targets: ${_all_library_targets}")
message(STATUS "All executable targets: ${_all_executable_targets}")
message(STATUS "All other targets: ${_all_other_targets}")

message(STATUS "Dude: We're done with ${CMAKE_CURRENT_LIST_FILE}.")
