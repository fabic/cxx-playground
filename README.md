# pimpl.io &ndash; Clang internals playground

Quick build command :

    $ ( [ -d build/ ] && rm -rf build/ || true) &&
        mkcd build &&
        ( cmake .. && make &&
            find -type f -perm -a+x -name \*.so -ls ) ; cd -

Usage :

    export CC="/usr/bin/clang -Xclang -load -Xclang `realpath build/PrintFunctionNames.so`"
    export CXX="/usr/bin/clang++ -Xclang -load -Xclang `realpath build/PrintFunctionNames.so`"

    $CXX -std=c++17 tutu/test-001.cpp
    $CXX -std=c++17 tutu/test-002.cpp
    $CXX -std=c++17 tutu/test-003.cpp

## ChangeLog

* __2017-12-16:__ Initial commit; Imported the PrintFunctionNames example from
  Clang; tampered with it a bit; set up a CMake `CMakeLists.txt` that feels
  more or less okay.

__*End of transmission*__

