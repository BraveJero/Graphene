#!/bin/bash

rm src/frontend/lexical-analysis/flex-scanner.c src/frontend/syntactic-analysis/bison-parser.*
rm -rf bin
cmake -S . -B bin
cd bin; make; cd ..