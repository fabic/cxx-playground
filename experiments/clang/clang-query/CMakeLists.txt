

llvm_map_components_to_libnames(
    LLVM_CLANG_LIBS
      LineEditor
      Support
  )

add_library(
  pgClangQuery
    SHARED
    Query.cpp
    QueryParser.cpp
  )

target_link_libraries(
  pgClangQuery

    ${LLVM_CLANG_LIBS}

    clangAST
    clangASTMatchers
    clangBasic
    clangDynamicASTMatchers
    clangFrontend
  )

add_subdirectory(tool)
