
message(STATUS "Hello! said “Clang PrintFunctionNames example (as-is)”.")

# add_llvm_loadable_module(PrintFunctionNames PrintFunctionNames.cpp PLUGIN_TOOL clang)
#
# if(LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
#   target_link_libraries(PrintFunctionNames PRIVATE
#     clangAST
#     clangBasic
#     clangFrontend
#     LLVMSupport
#     )
# endif()

add_library(PrintFunctionNames MODULE PrintFunctionNames.cpp)
target_link_libraries(PrintFunctionNames

  )

#dude_please_dump_cmake_variables()
