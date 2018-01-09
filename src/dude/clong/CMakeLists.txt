## clong-clang-plugin / 2017-12-16 / fabic.net

cmake_minimum_required(VERSION 3.3)

message(STATUS "~~ Dude, hi! ~~")

project(FabicClong)

set (FabicClong_VERSION_MAJOR 0)
set (FabicClong_VERSION_MINOR 3)

# TODO: Move stuff _out of here (!)_, and specifically: anything that may have
# TODO: side-effects on sub-projects we want to include with add_subdirectory()
# TODO: e.g. vendor/lmdb/



find_package(LLVM REQUIRED CONFIG)
# find_package(Clang REQUIRED)

# This would typically append for ex. '/usr/lib/cmake/llvm/' to CMake search path.
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

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


add_compile_options( -fPIC )


# set_property(DIRECTORY PROPERTY EXCLUDE_FROM_ALL ON)
# set(EXCLUDE_FROM_ALL ON)


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


