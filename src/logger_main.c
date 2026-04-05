#include <stdio.h>
#include "logger.h"

int main() {
    printf("=== Logger Module (Module 1) ===\n");
    printf("Logger ready. All modules log here: ../logs/logs.txt\n");
    
    write_log("LOGGER", "Logger module launched from main menu");
    write_log("LOGGER", "Logger module finished");
    
    printf("Press Enter to return...\n");
    getchar();
    return 0;
}
