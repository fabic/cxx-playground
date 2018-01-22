
cmake_minimum_required(VERSION 3.3)

#project(Surface)
#set (Surface_VERSION_MAJOR 0)
#set (Surface_VERSION_MINOR 1)

# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ DEPENDENCIES ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

find_package (
    Boost
    COMPONENTS REQUIRED
      log
      # program_options
      # filesystem
      # system
  )

## PKG-CONFIG auto-configuration “CMake bridge” # # # # # # # # # # # #
#
# CMake module: /usr/share/cmake/Modules/FindPkgConfig.cmake
# Cf also: https://wiki.gnome.org/Projects/gtkmm/UsingCMake
find_package(PkgConfig)

pkg_check_modules(XCB xcb)
# FYI: ^ redundant,
#        implied by xcb-aux it appears :
pkg_check_modules(XCB_AUX xcb-aux)

pkg_check_modules(CAIRO_XCB cairo-xcb)

pkg_check_modules(FT2 freetype2)


# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ TARGETS ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

add_executable( surface
    main.cpp
    #xcb/xcb.cpp
    #xcb/events.cpp
    #xcb/window.cpp
    cairo/surface.cpp
    ui/elements.cpp
    ui/surface.cpp
    text/TextBuffer.cpp
    text/TextBuffers.cpp
    text/Line.cpp ui/TextView.cpp
    #../hacks/hack_cxa_throw_exception.cpp
  )

target_compile_options( surface
    PUBLIC
    # ...
  )

target_include_directories( surface
  PUBLIC
    ${XCB_INCLUDE_DIRS}
    ${XCB_AUX_INCLUDE_DIRS}
    ${CAIRO_XCB_INCLUDE_DIRS}
  )

target_link_libraries( surface
    xcbx
    DudeHackRoutines
    ${Boost_LIBRARIES}
    ${XCB_LIBRARIES}
    ${XCB_AUX_LIBRARIES}
    ${CAIRO_XCB_LIBRARIES}
    unwind dl # for hack_cxa_throw_exception.cpp
    pthread
  )

set_property(TARGET surface
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

# TODO: fix these / 2018.
#add_subdirectory(tests)
