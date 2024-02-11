#!/bin/bash

clang -o build/parser src/*.c 

if [ $? -eq 0 ]; then
    ./build/parser
fi
