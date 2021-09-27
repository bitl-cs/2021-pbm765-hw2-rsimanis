#!/usr/bin/env bash

make --silent --no-print-directory -C ../.. all
num="$(echo "$1" | cut -d 't' -f 2)"
../../hw2.exe <"$1" >../outputs/output"$num"
make --silent --no-print-directory -C ../.. clean
