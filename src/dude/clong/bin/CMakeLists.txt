
add_executable(list-all-clang-decls-types
  list-all-clang-nodes.cpp
  $<TARGET_OBJECTS:PimplioTerminal>
  )

target_link_libraries(list-all-clang-decls-types
  DudeHackRoutines
  LLVMSupport
  clangAST
  clangBasic
  )

set_property(TARGET list-all-clang-decls-types
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
