#!/bin/sh

set -e

# Functions to separate actions in travis jobs
fold_start () { printf 'travis_fold:start:%s\r\033[33;1m%s\033[0m\n' "$1" "$2"; }
fold_end () { printf 'travis_fold:end:%s\r' "$1"; }


fold_start valgrind 'Make check with valgrind'
printf "\n== Compilation ==\n"
make 
printf "\n\n=== Valgrind Test ===\n"
make checkall
fold_end valgrind
