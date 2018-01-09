## clong-clang-plugin / 2017-12-16 / fabic.net

message(STATUS "~~ Dude: Ich bin Clong! ( ${CMAKE_CURRENT_SOURCE_DIR} ) ~~")

# TODO: Move stuff _out of here (!)_, and specifically: anything that may have
# TODO: side-effects on sub-projects we want to include with add_subdirectory()
# TODO: e.g. vendor/lmdb/

# Python is needed in order to produce a PrintFunctionNames.def file
# from the PrintFunctionNames.exports file. Found this somewhere in one of Clang's
# CMake files.
if (false)
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
endif()

add_definitions( ${LLVM_DEFINITIONS} )
include_directories( ${LLVM_INCLUDE_DIRS} )

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##

add_compile_options( -fPIC )

# set_property(DIRECTORY PROPERTY EXCLUDE_FROM_ALL ON)
# set(EXCLUDE_FROM_ALL ON)

# ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

add_subdirectory(experiments)

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

message(STATUS "~~ Dude: ^ Clong (end) ( ${CMAKE_CURRENT_SOURCE_DIR} ). ~~")
