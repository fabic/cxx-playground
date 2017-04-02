
# set(LLVM_LINK_COMPONENTS Support)
# add_clang_library(findAllSymbols
#     FindAllSymbols.cpp FindAllSymbolsAction.cpp FindAllMacros.cpp
#     HeaderMapCollector.cpp PathConfig.cpp PragmaCommentHandler.cpp
#     STLPostfixHeaderMap.cpp SymbolInfo.cpp
#   LINK_LIBS
#     clangAST clangASTMatchers clangBasic clangFrontend clangLex clangTooling
#   )

add_library(FindAllSymbols SHARED
    FindAllSymbols.cpp FindAllSymbolsAction.cpp FindAllMacros.cpp
    HeaderMapCollector.cpp PathConfig.cpp PragmaCommentHandler.cpp
    STLPostfixHeaderMap.cpp SymbolInfo.cpp
  )

target_link_libraries(FindAllSymbols
    clangAST clangASTMatchers clangBasic clangFrontend clangLex clangTooling
    LLVMSupport
  )

add_subdirectory(tool)
