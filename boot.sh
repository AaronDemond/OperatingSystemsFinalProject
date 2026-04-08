#!/bin/bash

rm -rf bin logs
echo "Cleaned old build and logs."

mkdir -p bin logs
echo "Created /bin and /logs directories."

gcc -o bin/logger src/logger.c
if [ $? -eq 0 ]; then echo "[OK] logger"; else echo "[FAIL] logger"; exit 1; fi

for module in file_management memory amdahl peterson main_menu; do
    if [ "$module" = "peterson" ]; then
        gcc -pthread -o "bin/$module" "src/$module.c"
    else
        gcc -o "bin/$module" "src/$module.c"
    fi

    if [ $? -eq 0 ]; then
        echo "[OK] $module"
    else
        echo "[FAIL] $module"
        exit 1
    fi
done

echo ""
echo "All modules compiled successfully."
echo "Starting main menu..."
echo ""

cd bin && ./main_menu

