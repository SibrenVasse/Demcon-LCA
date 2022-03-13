#!/bin/bash

# Test script for lca binary
# Run program with files inside input/ as stdin
# Use `diff` to test output of program against file in output/

if [ ! -f "$1" ]; then
    printf 'ERROR: %s does not exist.\n' "$1"
    exit 1
fi

for f in input/*; do
    printf "Running input for '%s'\n" "${f}"
    if ! output=$($1 < "${f}" |& diff -y "output/${f#input/}" -); then
        printf 'ERROR: Test failed for '%s'\n' "${f}"
        printf "%s" "${output}"
        exit 1
    fi
done
