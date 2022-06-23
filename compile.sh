#!/bin/bash

printf "Compiling\n"
i=$1
echo "cat "$i" | bin/Compiler &> /dev/null"
`cat "$i" | bin/Compiler &> /dev/null`
if [ $? = 0 ]; then
    printf "Compilation successful\n"
else
    printf "Compilation failed\n"
fi