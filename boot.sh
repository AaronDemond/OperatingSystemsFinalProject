#!/bin/bash

echo "Cleaning previous build..."

rm -rf bin
rm -rf logs

mkdir bin
mkdir logs

echo "Compiling logger..."
gcc src/logger.c -o bin/logger

echo "Compiling modules..."

gcc src/file_management.c -o bin/file_management
gcc src/amdahls_law.c -o bin/amdahl
gcc src/mem_alloc.c -o bin/memory
gcc src/peterson.c -lpthread -o bin/peterson
gcc src/main_menu.c -o bin/main_menu

echo "Running system..."

cd bin
./main_menu
