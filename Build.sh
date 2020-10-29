#!/usr/bin/env bash
#
# Script file to compile all C++ source files in or under the
# current directory.  This has been used in the OpenSUSE and Ubuntu
# environments with the GCC and Clang compilers and linkers
executableFileName="${1:-project}"

# Find and display all the C++ source files to be compiled ...
# temporarily ignore spaces when globing words into file names
temp=$IFS
  IFS=$'\n'
  sourceFiles=( $(find ./ -name "*.cpp") ) # create array of source files
IFS=$temp

echo "compiling ..."
for fileName in "${sourceFiles[@]}"; do
  echo "  $fileName"
done
echo ""


#define options
GccOptions="  -Wall -Wextra -pedantic        \
              -Wdelete-non-virtual-dtor      \
              -Wduplicated-branches          \
              -Wduplicated-cond              \
              -Wextra-semi                   \
              -Wfloat-equal                  \
              -Winit-self                    \
              -Wlogical-op                   \
              -Wnoexcept                     \
              -Wnon-virtual-dtor             \
              -Wold-style-cast               \
              -Wstrict-null-sentinel         \
              -Wsuggest-override             \
              -Wswitch-default               \
              -Wswitch-enum                  \
              -Woverloaded-virtual           \
              -Wuseless-cast                 \
              -Wzero-as-null-pointer-constant"


ClangOptions=" -stdlib=libc++ -Weverything    \
               -Wno-comma                     \
               -Wno-unused-template           \
               -Wno-sign-conversion           \
               -Wno-exit-time-destructors     \
               -Wno-global-constructors       \
               -Wno-missing-prototypes        \
               -Wno-weak-vtables              \
               -Wno-padded                    \
               -Wno-double-promotion          \
               -Wno-c++98-compat-pedantic     \
               -Wno-c++11-compat-pedantic     \
               -Wno-c++14-compat-pedantic     \
               -Wno-c++17-compat-pedantic     \
               -fdiagnostics-show-category=name"

CommonOptions="-g0 -O3 -DNDEBUG -pthread -std=c++17 -I./ -DUSING_TOMS_SUGGESTIONS -D__func__=__PRETTY_FUNCTION__"




ClangCommand="clang++ $CommonOptions $ClangOptions"
echo $ClangCommand
clang++ --version
if $ClangCommand -o "${executableFileName}_(clang++).exe"  "${sourceFiles[@]}"; then
  echo -e "\nSuccessfully created  \"${executableFileName}_(clang++).exe\""
else
  exit 1
fi

echo ""

GccCommand="g++ $CommonOptions $GccOptions"
echo $GccCommand
g++ --version
if $GccCommand  -o "${executableFileName}_(g++).exe"  "${sourceFiles[@]}"; then
   echo -e "\nSuccessfully created  \"${executableFileName}_(g++).exe\""
else
   exit 1
fi
