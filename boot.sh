#!/bin/bash
# boot.sh - CSCI 3431 Group Project Build Script

echo "=== CSCI 3431 Group Project Build ==="

# 1. Clean old build
rm -rf bin logs
echo "Cleaned old build"

# 2. Create folders
mkdir -p bin logs
echo "Created bin/ and logs/ folders"

# 3. Compile Logger Module FIRST (creates /bin/logger executable)
echo "Compiling logger module..."
if gcc -Wall src/logger.c src/logger_main.c -o bin/logger; then
    echo "Logger compiled successfully (/bin/logger)"
else
    echo "Logger compilation failed"
    exit 1
fi

# 4. Compile other modules (each links with logger.c)
echo "Compiling file_management..."
gcc -Wall src/file_management.c src/logger.c -o bin/file_management 2>/dev/null || echo "file_management failed"

echo "Compiling peterson..."
gcc -Wall src/peterson.c src/logger.c -o bin/peterson 2>/dev/null || echo "peterson failed"

echo "Compiling memory..."
gcc -Wall src/memory.c src/logger.c -o bin/memory 2>/dev/null || echo "memory failed"

echo "Compiling amdahl..."
gcc -Wall src/amdahl.c src/logger.c -o bin/amdahl 2>/dev/null || echo "amdahl failed"

echo "Compiling main_menu..."
gcc -Wall src/main_menu.c src/logger.c -o bin/main_menu 2>/dev/null || echo "main_menu failed"

echo ""
echo "=== Build complete. Starting main menu ==="
echo "Check logs/logs.txt for all activity."
echo ""
cd bin
./main_menu
