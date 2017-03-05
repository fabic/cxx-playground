#!/bin/sh

rewt=$( cd `dirname "$0"` && pwd )

# Move up out of bin/
cd "$rewt" || exit 1

localdir="$(cd "local" && pwd)"
llvmClang="$(cd "llvm-clang" && pwd)"
misc="$(cd "misc" && pwd)"

echo "+-- $0"
echo "| In $rewt"
echo "|"

locations=(
    "$rewt/include"
    "$rewt/src"
  )

# BOOST C++ !
if [ ! -z "$BOOSTROOT" ] && [ -d "$BOOSTROOT/include" ];
then
  echo "| Got Boost C++ at \$BOOSTROOT = '$BOOSTROOT'"
  locations=(
      ${locations[@]}
      "$BOOSTROOT/include"
    )
fi

find_args=(
    "${locations[@]}"
    -type d \( -name .git -o -name .svn \) -prune
      -o \( -type f -iregex '.+\.\(c\|h\|hh\|cpp\|hpp\|cxx\|hxx\|h\.inc\|s\)$' \) \
        -print
  )

echo "| Generating the cscope.files (\`find ...) :\`"
echo "|"
echo "|   find \"${find_args[@]}\" | sort > cscope.files"
echo "|"

find "${find_args[@]}" \
  | sort \
    > cscope.files

retv=$?

if [ $retv -ne 0 ];
then
  echo
  echo "| \`find ... \` command failed with exit status $retv"
  exit $retv
fi


echo "+-"
echo "| Found $(wc -l cscope.files | awk '{print $1}' ) source files (cscope.files)."
echo "|"
read -p "| Run CScope on these ? (Ctrl-C to abort)"


echo "|"
echo "| Running \`cscope -b -q\`"

time \
  cscope -b -q

if [ $retv -ne 0 ];
then
  echo
  echo "| \`cscope ... \` command failed with exit status $retv"
  exit $retv
fi


echo "| Cscope files :"
echo "|"

ls -lh cscope.* | sed -e 's/^/|  &/'


echo "+-- $0 : END, exit status: $retv"

exit $retv
