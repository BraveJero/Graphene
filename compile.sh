#!/bin/bash

printf "Compiling\n"
i=$1
`cat "usecases/r8_$i.gene" | bin/Compiler &> /dev/null`
printf "Compilation finished\n"
