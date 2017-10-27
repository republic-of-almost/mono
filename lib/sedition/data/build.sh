
clang ../src/interface/sed.c ../src/internal/array.c ../src/internal/common.c ../src/platform/xcode.c -I ../include/ -I ../src/ -std=C89 -ansi -g -o sed_build
