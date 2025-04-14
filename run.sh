#!/usr/bin/env bash
program="bin/db.out"
comman_cmd="$program examples/00.p"
set -ex
if [ "$1" == "-val" ]; then
    make -B DEBUG=1 && valgrind --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --verbose \
        --log-file=valgrind-out.txt \
        $comman_cmd
elif [ "$1" == "-gdb" ]; then
    make -B DEBUG=1 && gdb $program
elif [ "$1" == "-read" ]; then
    make -B READ=1 && $program
else
    make -B LOG=1 && $comman_cmd
fi
