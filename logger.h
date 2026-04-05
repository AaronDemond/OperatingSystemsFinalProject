#ifndef LOGGER_H
#define LOGGER_H

void write_log(const char *module, const char *message);
void write_log_input(const char *module, const char *field, const char *value);

#endif
