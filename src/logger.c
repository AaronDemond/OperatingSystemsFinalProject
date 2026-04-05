/*
 * Module 1: Logging System (IPC) - Helper Functions
 * Purpose: Centralized logging functions for all modules.
 * All modules compile with this file linked.
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "logger.h"

void write_log(const char *module, const char *message) {
    FILE *fp;
    time_t now;
    struct tm *time_info;
    char timestamp[64];

    fp = fopen("../logs/logs.txt", "a");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Cannot open log file\n");
        return;
    }

    time(&now);
    time_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

    fprintf(fp, "[%s] [%s] %s\n", timestamp, module, message);
    fflush(fp);
    fclose(fp);
}

void write_log_input(const char *module, const char *field, const char *value) {
    char message[256];
    snprintf(message, sizeof(message), "INPUT %s = %s", field, value);
    write_log(module, message);
}
