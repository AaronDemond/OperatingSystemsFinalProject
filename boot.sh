#!/bin/bash

rm -rf bin logs
echo "Cleaned old build and logs."

mkdir -p bin logs
echo "Created /bin and /logs directories."

gcc -Wall -Wextra -o bin/logger src/logger.c
if [ $? -eq 0 ]; then echo "[OK] logger"; else echo "[FAIL] logger"; exit 1; fi

for module in file_management peterson memory amdahl main_menu; do
    gcc -Wall -Wextra -o "bin/$module" "src/$module.c"
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
