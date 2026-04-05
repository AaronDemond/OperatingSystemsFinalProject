/*
 * Module 1: Logging System (IPC)
 */

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "logger.h"

static void vlog_write(const char *module, const char *level, const char *format, va_list args) {
    FILE *fp;
    time_t now;
    struct tm *time_info;
    char timestamp[64];
    char buffer[1024];

    fp = fopen("../logs/logs.txt", "a");
    if (fp == NULL) return;

    time(&now);
    time_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

    vsnprintf(buffer, sizeof(buffer), format, args);

    fprintf(fp, "[%s] [%s:%s] %s\n", timestamp, module, level, buffer);
    fclose(fp);
}

void log_info(const char *module, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vlog_write(module, "INFO", format, args);
    va_end(args);
}

void log_error(const char *module, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vlog_write(module, "ERROR", format, args);
    va_end(args);
}

int main() {
    printf("=== Logger Module Ready ===\n");
    log_info("LOGGER", "Logger started - supports any data type!");
    printf("Press Enter to exit...\n");
    getchar();
    return 0;
}
