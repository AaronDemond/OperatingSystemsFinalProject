#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
    FILE *log = fopen("../logs/logs.txt", "a");

    if (!log)
    {
        perror("Log file error");
        return 1;
    }

    char buffer[512];

    while (fgets(buffer, sizeof(buffer), stdin))
    {
        time_t now = time(NULL);
        char *t = ctime(&now);
        t[strlen(t) - 1] = '\0';

        fprintf(log, "[%s] %s", t, buffer);
        fflush(log);
    }

    fclose(log);
    return 0;
}
