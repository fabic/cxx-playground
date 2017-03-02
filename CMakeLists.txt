# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
# @link http://www.vtk.org/Wiki/CMake_Useful_Variables

cmake_minimum_required(VERSION 3.3)


## CCACHE
# https://crascit.com/2016/04/09/using-ccache-with-cmake/
find_program(CCACHE_PROGRAM ccache)
if (false AND CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  message(STATUS "Dude! Found ccache, so we're setting CMake var. RULE_LAUNCH_COMPILE to '${CCACHE_PROGRAM}'.")
endif()


## Add new `` MinSizeDeb `` build type configuration
#
# https://cmake.org/Wiki/CMake_FAQ#How_can_I_specify_my_own_configurations_.28for_generators_that_allow_it.29_.3F
# https://cmake.org/Wiki/CMake_FAQ#How_can_I_extend_the_build_modes_with_a_custom_made_one_.3F

# 1st add it to the list of configuration types [optional it appears (?)]
# (note that it may not be supported by CMake, seamingly, see FAQ).
if(CMAKE_CONFIGURATION_TYPES)
  list(APPEND CMAKE_CONFIGURATION_TYPES MinSizeDeb )
  list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
    "Add the configurations that we need, dah!" FORCE)
endif()

# There it is.
set( CMAKE_C_FLAGS_MINSIZEDEB "-Os -g" CACHE STRING
  "Optimize for size, with debug informations (C compiler flags)." FORCE )
set( CMAKE_CXX_FLAGS_MINSIZEDEB "-Os -g" CACHE STRING
    "Optimize for size, with debug informations (C++ compiler flags)." FORCE )

set( CMAKE_EXE_LINKER_FLAGS_MINSIZEDEB
    "-Wl,--warn-unresolved-symbols" CACHE STRING
    "Flags used for linking executables (MinSizeDev)." FORCE )
set( CMAKE_SHARED_LINKER_FLAGS_MINSIZEDEB
    "-Wl,--warn-unresolved-symbols" CACHE STRING
    "Flags used by the shared libraries linker (MinSizeDeb)." FORCE )


## Set default build type.
# https://cmake.org/Wiki/CMake_FAQ#How_can_I_change_the_default_build_mode_and_see_it_reflected_in_the_GUI.3F
IF(NOT CMAKE_BUILD_TYPE)
  #SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
  SET(CMAKE_BUILD_TYPE MinSizeDeb CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel MinSizeDeb."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)


## Clang link-time-optimization thing (LTO).
option(DUDE_LETS_TRY_LTO
  "Will pass -flto to Clang (requires among other things: Binutils' ld.gold and LLVM/Clang built with LTO support)." OFF)


# ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`


project(FabicCppWanderings)

set (FabicCppWanderings_VERSION_MAJOR 0)
set (FabicCppWanderings_VERSION_MINOR 3)


## FHS-style local/ directory.
#
  if (NOT DEFINED FABIC_LOCAL_DIR)
    set(FABIC_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../local)
    message(STATUS "Trying FHS-style local/ directory (guessed) : '${FABIC_LOCAL_DIR}'.")
  else()
    message(STATUS "FHS-style local/ directory set to ${FABIC_LOCAL_DIR}")
  endif()

  if (NOT IS_DIRECTORY ${FABIC_LOCAL_DIR})
    message(WARNING "FHS-like “local/” directory doesn't exist.")
  else()
    message(STATUS "FHS-style local/ directory exists, ok.")
  endif()
# ^ local/ FHS-style setup.
##


### CMake extra local/lib/cmake/ modules path ?
# For LLVM/Clang CMake modules

if (IS_DIRECTORY "${FABIC_LOCAL_DIR}/lib/cmake")
    list(APPEND CMAKE_MODULE_PATH "${FABIC_LOCAL_DIR}/lib/cmake")
    message(STATUS "LLVM/Clang: Prepended CMAKE_MODULE_PATH with '${FABIC_LOCAL_DIR}/lib/cmake'.")
endif()

message(STATUS "FYI: CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")


## PKG-CONFIG auto-configuration “CMake bridge” # # # # # # # # # # # #
#
# CMake module: /usr/share/cmake/Modules/FindPkgConfig.cmake
# Cf also: https://wiki.gnome.org/Projects/gtkmm/UsingCMake
find_package(PkgConfig)


##
# This will cause `-I.` and similar/redondant directives like `-I../symfony/..`
# to be emitted (for systematic ability of #include-ing files from the
# “current source file” directory)
#
# EDIT/2016-07-27 : Enabling thing project-wide, having definitions (headers)
#                   separated from the sources provide no added value at the
#                   moment, and gets in my way.
set(CMAKE_INCLUDE_CURRENT_DIR ON)


### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
### #  COMPILER FLAGS, DEFINES, ETC...  # # # # # # # # # # # # # # # #
### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Top-level namespace #define-s
# FIXME: drop this.
add_definitions(
  -DTL_NS_BEGIN=
  -DTL_NS_END=
  -DTLNS=
  )


# Replacing these `set ...` by add_compile_options(...) :
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")


add_compile_options(
  -std=c++1z
  # -stdlib=libc++
  -fPIC
  #-flto
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

# add_compile_options( -flto )
# ^ requires LLVMgold.so, missing on my Sabayon system (Clang 3.8).
#   http://clang.llvm.org/docs/LTOVisibility.html


add_compile_options(
  # -O0 -ggdb
  #      ^ implies -g
  # ^ this is automatically handled by CMake build type thing.
  # So as to get better/meaninful stack traces in error messages :
  -fno-omit-frame-pointer
  # and disable tail call elimination
  -fno-optimize-sibling-calls
  )


add_compile_options(
  # -fsanitize=address     # http://clang.llvm.org/docs/AddressSanitizer.html
  # ^ -fsanitize=memory
  # ^ -fsanitize=thread
  #   “ It is not possible to combine more than one of the
  #     -fsanitize=address, -fsanitize=thread, and -fsanitize=memory
  #     checkers in the same program. ”
  #     http://clang.llvm.org/docs/UsersManual.html#controlling-code-generation
  # -fsanitize=undefined   # http://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
  # -fsanitize=dataflow    # http://clang.llvm.org/docs/DataFlowSanitizer.html
  # -fsanitize=cfi -flto   # http://clang.llvm.org/docs/ControlFlowIntegrity.html (requries -flto)
  # -fsanitize=safe-stack  # http://clang.llvm.org/docs/SafeStack.html
  )


add_compile_options(
  # -fvisibility-inlines-hidden
  # -Wwrite-strings
  # -Wcast-qual
  -Wmissing-field-initializers
  # -Wcovered-switch-default
  #   ^ we get too many warnings from external headers.
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


## ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
## ~ ~ ~ ~ LTO ~ ~ ~ ~
## ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
find_program(LD_GOLD_PROGRAM ld.gold)

if (LD_GOLD_PROGRAM)
  message(STATUS "Dude! Found Binutils' « ld.gold » linker (which supports LTO and the `-plugin ...` argument, btw).")
  # message(STATUS "Setting CMAKE_LINKER = '${LD_GOLD_PROGRAM}'")
  # set( CMAKE_LINKER "${LD_GOLD_PROGRAM}" )
  # ^ DON'T: enabling it for non-LTO compilation/linkage appears to fail with :
  #          “main.cpp.o: unrecognized file format”
endif()

if (DUDE_LETS_TRY_LTO)
  if (LD_GOLD_PROGRAM)
    message(STATUS "Setting CMAKE_LINKER = '${LD_GOLD_PROGRAM}'")
    set( CMAKE_LINKER "${LD_GOLD_PROGRAM}" )

    add_compile_options( -flto )
    link_libraries( -flto )
    message(STATUS "Yeah man! Setting the -flto Clang compiler flag, fingers-crossed o_o")
    message(STATUS "          Setting the -flto linker flag too.")

    # Replace -Os with -O2 from the CXXFLAGS.
    # FIXME: We're not altering the CFLAGS.
    if (true)
      string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
      #message(STATUS "HUH! ${CMAKE_CXX_FLAGS} -- ${uppercase_CMAKE_BUILD_TYPE} --- ${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}}")
      if ( ${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}} MATCHES "-Os" )
        message(STATUS "Beware! Clang won't let us compile with -Os while doing LTO (at least not as of February 2017).")
        string(REPLACE "-Os" "-O2" CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE} "${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}}")
        message(STATUS "Replaced -Os with -O2 from CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE} : ${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}}.")
      endif()
      unset( uppercase_CMAKE_BUILD_TYPE )
    endif()
  else()
    message(WARNING "Argh! dude :-| Couldn't find the `ld.gold` binary (well CMake didn't, I'm cool) ; we're _not_ passing -flto.")
  endif()
endif()


### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Boost C++
#
# https://cmake.org/cmake/help/latest/module/FindBoost.html
# https://svn.boost.org/trac/boost/wiki/CMakeBuildFeatures

  # Hard-coded path if needed for `find_package(Boost...)`
  #   - or simply set this as an environment variable,
  #   - or pass `-DBOOSTROOT=...` when running CMake.
  # (works ok with my CMake v3.4.3)
  if (NOT IS_DIRECTORY "$ENV{BOOSTROOT}")
    set(BOOSTROOT ../local/boost-1.61.0-clang)
    message(WARNING "Boost C++: using hard-coded path for BOOSTROOT = ${BOOSTROOT}.")
  elseif(IS_DIRECTORY $ENV{BOOSTROOT})
    set(BOOSTROOT $ENV{BOOSTROOT})
    message(STATUS "Boost C++ location said to be at ${BOOSTROOT}")
  else()
    message(WARNING "Boost C++ location $ENV{BOOSTROOT} ain't no exit buddy.")
  endif()

  # This one (with the '_' blankee) is for JetBrains' CLion bundled CMake (v3.5.2) :
  set(BOOST_ROOT ${BOOSTROOT})

  # These two ain't needed, actually...
  # set(BOOST_INCLUDE_DIRS ${BOOSTROOT}/include)
  # set(BOOST_LIBRARY_DIRS ${BOOSTROOT}/local/boost-1.61.0-clang/lib)

  #set (Boost_DEBUG)
  # ^ this turns out debug output _ONLY FOR_ the CMake module !

  set (Boost_USE_MULTITHREAD ON)  # enable multithreading
  set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking

  #add_definitions(-DBOOST_ALL_DYN_LINK)
  # ^ solves linking problems, notably with Boost.log library.
  #   See http://www.boost.org/doc/libs/1_61_0/libs/log/doc/html/log/rationale/namespace_mangling.html
  #   See also `symfony/include/fabic/logging.hpp`

  #   Note that the following would also solve the
  #     “ unresolved symbol `v2s_mt_posix` ”
  #   problem for the `log` module only.
  add_definitions(-DBOOST_LOG_DYN_LINK)

  find_package(Boost 1.61.0 REQUIRED)

  if (Boost_FOUND)
  	include_directories(${Boost_INCLUDE_DIR})
    # find_package (
    #   Boost
    #   COMPONENTS REQUIRED
    #     log
    #     program_options
    #     filesystem
    #     system
    # )
    # ^ now performed where/when needed.
  else()
    message(WARNING "Boost C++: Ain't not found lad :-/")
  endif (Boost_FOUND)
# ^ Boost C++
###


###
# cpp-netlib
#
# For the HTTP module. Defined here instead of within `http/CMakeLists.txt`
# so that we may eventually have other modules based on cpp-netlib.
#
# Defined here :
#   - ${LibCppNetlibClientConnections}
#   - ${LibCppNetlibServerParsers}
#
# FIXME: hard-coded paths, can't get my way 'round those CMake stuff...
if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-netlib/boost)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-netlib)

  find_file(
    LibCppNetlibClientConnections
    libcppnetlib-client-connections.so
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-netlib/build/libs/network/src
    )

  find_file(
    LibCppNetlibServerParsers
    libcppnetlib-server-parsers.so
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-netlib/build/libs/network/src
    )
endif()
# ^ cpp-netlib
##


include_directories(include)
# ^ NOTE/2016-07-27: emptied the include/ dir. in favor of having headers reside
#                    along implementations.
# ^ and have src/ as include dir. instead :
include_directories(src)

add_subdirectory(experiments)

add_subdirectory(lib)

add_subdirectory(src)


### DUMP ALL VARIABLES (DEBUG) ##############################################
##
# http://stackoverflow.com/questions/9298278/cmake-print-out-all-accessible-variables-in-a-script

  if (false)
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
     message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
  endif (false)

##
### ^ CMake defined variables debug dump. ###################################
