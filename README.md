# cxx-playground

[fabic.net](https://fabic.net)'s personal C++ playground.

### Build instructions

Quick build command :

    $ ( [ -d build/ ] && rm -rf build/ || true) &&
        mkcd build &&
        ( cmake .. && make &&
            find -type f -perm -a+x -name \*.so -ls ) ; cd -

### Pointers

* PostgreSql's [libpqxx](http://pqxx.org/development/libpqxx/) &ndash; [libpqxx.readthedocs.io](http://libpqxx.readthedocs.io/en/latest/)

### ChangeLog

* __2018-02-17:__ Been a month since I've touched any C++, lost momentum with that
  Clong plugin :-/
* __2018-01-17:__ Dude, you were working on that _Clong_ Clang plugin thing;
 __status:__ partially working proof-of-concept, this may lead somewhere.


__EOF__
