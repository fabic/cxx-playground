# cxx-playground/lib/CMakeLists.txt -- fabic.net

##
## See lib/fltk/CMake/options.cmake
##
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fltk/CMakeLists.txt)
  message(STATUS "FLKT: Found, we'll build it along...")
#  option(OPTION_USE_POLL     "use poll if available" ON)
  option(OPTION_BUILD_SHARED_LIBS "Build shared libraries (in addition to static libraries)" ON)

  option(OPTION_BUILD_EXAMPLES "build example programs" OFF)
#  option(OPTION_BUILD_HTML_DOCUMENTATION "build html docs" ON)

  option(OPTION_CAIRO    "use lib Cairo" ON)
#  option(OPTION_CAIROEXT "use FLTK code instrumentation for cairo extended use" ON)

  option(OPTION_USE_GL      "use OpenGL"                OFF)
  option(OPTION_USE_THREADS "use multi-threading"       OFF)
  option(OPTION_LARGE_FILE  "enable large file support" OFF)

  ## option(OPTION_USE_SYSTEM_ZLIB    "use system zlib"    ON)
  ## option(OPTION_USE_SYSTEM_LIBJPEG "use system libjpeg" ON)
  ## option(OPTION_USE_SYSTEM_LIBPNG  "use system libpng"  ON)

#  option(OPTION_USE_XINERAMA "use lib Xinerama" OFF)
#  option(OPTION_USE_XFIXES   "use lib Xfixes"   OFF)
#  option(OPTION_USE_XCURSOR  "use lib Xcursor"  OFF)
#  option(OPTION_USE_XFT      "use lib Xft"      OFF)
#  option(OPTION_USE_XRENDER  "use lib Xrender"  OFF)
#  option(OPTION_USE_XDBE     "use lib Xdbe"     OFF)

  add_subdirectory(fltk)
else()
  message(STATUS "Dude: FTLK not found, ok.")
endif ()

##
## LMDB key-value store database from the OpenLDAP project.
##
## Note: it happens by pure coincidence, that the output dir.
##       will be build/lib/  (because _this_ dir. is 'lib/').
##       -> so we're not overriding this target's output dir.
##
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lmdb/libraries/liblmdb/mdb.c)
  message(STATUS "Dude! Found LMDB sources, adding custom library target for building it, ok.")
  add_library(lmdb STATIC lmdb/libraries/liblmdb/mdb.c
                          lmdb/libraries/liblmdb/midl.c )
  target_compile_options(lmdb PRIVATE -fPIC)
  target_include_directories(lmdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/lmdb/libraries/liblmdb> )
  # ^ this will have dependent targets automagically have
  #   the correct -I include path be set for them.
else()
  message(STATUS "Dude, didn't find LMDB sources, skipping.")
  message(STATUS "      ` see ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt about this.")
endif()
