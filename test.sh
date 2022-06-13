#!/bin/bash

NC='\033[0m' # No Color
RED='\033[0;31m'
GREEN='\033[0;32m'

printf "Testing usecases\n"

for i in {0..10}
do
    printf "Testing usecase #%02d ... " $i
    `cat "usecases/r8_$i.gene" | bin/Compiler &> /dev/null`
    if [ $? = 0 ]; then
        printf "${GREEN}Accepted${NC}\n"
    else
        printf "${RED}Rejected${NC}\n"
    fi
done

for i in {11..15}
do
    printf "Testing usecase #%02d ... " $i
    `cat "usecases/r8_$i.gene" | bin/Compiler &> /dev/null`
    if [ $? -ne 0 ]; then
        printf "${GREEN}Rejected${NC}\n"
    else
        printf "${RED}Accepted${NC}\n"
    fi
done

printf "Testing finished\n"