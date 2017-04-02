
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(find-all-symbols FindAllSymbolsMain.cpp)

target_link_libraries(find-all-symbols
    clangAST clangASTMatchers clangBasic clangFrontend clangLex clangTooling
    LLVMOption LLVMSupport
    findAllSymbols
  )

