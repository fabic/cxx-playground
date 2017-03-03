
cmake_minimum_required(VERSION 3.3)

project(Frog)

set (Frog_VERSION_MAJOR 0)
set (Frog_VERSION_MINOR 1)

add_definitions()

add_compile_options(
    -std=c++1z
    -stdlib=libc++
      -fPIC
      -Wall
      -pedantic
        -fcolor-diagnostics
        -fdiagnostics-show-category=name
        -fdiagnostics-show-template-tree
          -fno-omit-frame-pointer
          -fno-optimize-sibling-calls
            -Wmissing-field-initializers
            -Wcovered-switch-default
            -Wnon-virtual-dtor
            -Wdelete-non-virtual-dtor
            -Woverloaded-virtual
            -fcolor-diagnostics
  )


# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
# DEPENDENCIES
# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
find_package (
    Boost
    COMPONENTS REQUIRED
      log
      program_options
      filesystem
      system
  )


# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
# TARGETS
# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

add_executable( frog
    main.cpp
    ../hacks/hack_cxa_throw_exception.cpp
  )

target_link_libraries( frog
    ${Boost_LIBRARIES}
    unwind dl # for hack_cxa_throw_exception.cpp
    pthread
  )

