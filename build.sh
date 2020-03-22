#!/bin/sh
set -eu
cd "$(dirname "$0")"
set -x
clang -Werror -Weverything -Wno-missing-noreturn -fsanitize=address -g \
    -o test test.c unix.c
