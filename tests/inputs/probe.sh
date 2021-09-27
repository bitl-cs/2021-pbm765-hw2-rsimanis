#!/usr/bin/env bash

make --silent --no-print-directory -C ../.. all
../../hw2.exe <"$1"
make --silent --no-print-directory -C ../.. clean
