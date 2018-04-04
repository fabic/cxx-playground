# src/dude/hacks/CMakeLists.txt // fabic.net

find_package( PkgConfig )
pkg_check_modules( LIBUNW libunwind )

if (NOT LIBUNW_FOUND)
  message( WARNING "Dude: Couldn't find libunwind through pgk-config (!)." )
else()
  message( STATUS "Dude/hacks: Got libunwind" )
  message( STATUS "          \` LIBUNW_LIBDIR     = ${LIBUNW_LIBDIR}" )
  message( STATUS "          \` LIBUNW_LDFLAGS    = ${LIBUNW_LDFLAGS}" )
  message( STATUS "          \` LIBUNW_INCLUDEDIR = ${LIBUNW_INCLUDEDIR}" )
  #dude_dump_cmake_variables_matching("LIBUNW.*")

  # TODO: make it a static library so that we can define dependent libraries with INTERFACE...
  add_library( DudeHackCxaThrowException OBJECT hack_cxa_throw_exception.cpp )
  target_include_directories(DudeHackCxaThrowException PUBLIC "${LIBUNW_INCLUDEDIR}")

  # add_library(LocalLibunwindStatic STATIC IMPORTED)
  # set_target_properties(LocalLibunwindStatic PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/local/lib/libunwind-x86_64.a)
  # ^ should have worked, didn't.
  # ^ TODO: find out how to declare and use imported library targets.

  # add_library( DudeHackRoutines OBJECT dude/hacks/routines.cpp )
  add_library( DudeHackRoutines STATIC routines.cpp )
  target_include_directories(DudeHackRoutines PUBLIC "${LIBUNW_INCLUDEDIR}")

  # For targets that link against our DudeHackRoutines static lib.,
  # the following statement will have CMake automatically link against
  # those two statics that we need from libunwind :
  target_link_libraries( DudeHackRoutines INTERFACE
    -L "${LIBUNW_LIBDIR}" ${LIBUNW_LDFLAGS}
    unwind-x86_64
    # ^ FIXME: for some reason we have to specify this one, why?

    # When you build your own and use the static libs.:
    # ${CMAKE_SOURCE_DIR}/local/lib/libunwind-x86_64.a
    # ${CMAKE_SOURCE_DIR}/local/lib/libunwind.a
    #unwind-x86_64.a
    #unwind.a
    #lzma
    )
endif()

## EOF ##
