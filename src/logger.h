#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

static inline void log_event_internal(const char *file, int line, const char *fmt, ...)
{
    char *fd_str = getenv("LOGGER_FD");
    if (!fd_str)
        return;

    int fd = atoi(fd_str);

    char msg[400];
    char final[512];

    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    snprintf(final, sizeof(final), "[%s:%d] %s\n", file, line, msg);

    write(fd, final, strlen(final));
}

#define LOG(msg) log_event_internal(__FILE__, __LINE__, "%s", msg)
#define LOGF(fmt, ...) log_event_internal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif
